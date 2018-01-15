#include "CNetworkManager.h"
#include "NetworkUpdateClasses.h"
#include "../../../Game/Objects/Units/CUnitObject.h"
#include "../../../Game/Unit Commands/CMoveCommand.h"
#include "../../../Game/Objects/Units/CProjectile.h"
#include <string.h>
#include <iostream>

volatile CNetworkManager* CNetworkManager::m_Instance = NULL;
LPCRITICAL_SECTION CNetworkManager::m_pCS = NULL;
//sckt::Library* sckt::Library::instance = NULL;


CNetworkManager::CNetworkManager()
{
	m_ConnectedIP = NULL;
	m_UDPListen.Open(PORT_NUMBER);
	m_UDPSend.Open();

	m_RetryList.reserve(50);

	m_RetryMessageTotal = 1;
	m_SharedIDTotal = 1;

	m_MaxRetry = 200;
	m_RetryTimer = 0;

	running = true;
}

CNetworkManager::~CNetworkManager()
{
	for(unsigned int i = 0; i < m_RetryList.size(); i++)
	{
		delete m_RetryList[i];
	}
	m_RetryList.clear();
}

void CNetworkManager::Lock()
{
	EnterCriticalSection(m_pCS);
}

void CNetworkManager::Unlock()
{
	LeaveCriticalSection(m_pCS);
}

CNetworkManager* CNetworkManager::GetInstance()
{
	if(!m_Instance)
	{
		m_Instance = new CNetworkManager;
		
	}
	return const_cast<CNetworkManager*>(m_Instance);
}

void CNetworkManager::DeleteInstance()
{
	delete m_Instance;
	m_Instance = NULL;
}

void CNetworkManager::Shutdown()
{
	if(m_ConnectedIP)
	{
		CNetworkManager::GetInstance()->SendDisconnectPacket();
		delete m_ConnectedIP;
		m_ConnectedIP = NULL;
	}
	running = false;
}

bool CNetworkManager::CheckRunning()
{
	return running;
}

void CNetworkManager::SetConnectedIP(char* _IP)
{
	if(m_ConnectedIP)
	{
		delete m_ConnectedIP;
		m_ConnectedIP = NULL;
	}
	

	if(_IP)
	{
		m_ConnectedIP = new sckt::IPAddress(_IP, PORT_NUMBER);
	}
}

bool CNetworkManager::IsConnected()
{
	if(m_ConnectedIP)
	{
		if(m_ConnectedIP->host != 0)
			return true;
		else
			return false;
	}
	else 
		return false;
}

void CNetworkManager::SendCreateUnitPacket(CUnitObject* _unit, char* _str)
{
	try
	{
		if(IsConnected())
		{
			CCreateUnitPacket* sendUnit = new CCreateUnitPacket;
			_unit->SetSharedID(m_SharedIDTotal++);
			sendUnit->m_UpdateType = NW_CREATEUNIT;
			//ID for validation
			sendUnit->m_MessageID = m_RetryMessageTotal++;
			strcpy_s(sendUnit->createString, 32, _str);
			sendUnit->posx = (short)_unit->GetPositionX();
			sendUnit->posy = (short)_unit->GetPositionY();
			sendUnit->shareID = _unit->GetSharedID();
			sendUnit->attack = _unit->GetBaseAttack();
			sendUnit->defense = _unit->GetBaseDefense();
			sendUnit->evade = _unit->GetBaseEvade();
			sendUnit->movementSpeed = _unit->GetBaseMovement();
			sendUnit->attackSpeed = _unit->GetBaseAttackSpeed();

			//Send and add to retry
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, sendUnit, sizeof(CCreateUnitPacket));
			m_RetryList.push_back(sendUnit);
			m_UDPSend.Send(buf, sizeof(CCreateUnitPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendUnitUpdatePacket(CUnitObject* _unit)
{
	try
	{
		if(IsConnected())
		{
			CUpdateUnitPacket* sendUnit = new CUpdateUnitPacket;
			sendUnit->m_UpdateType = NW_UPDATEUNIT;
			sendUnit->posx = (short)_unit->GetPositionX();
			sendUnit->posy = (short)_unit->GetPositionY();
			sendUnit->velx = (short)_unit->GetVelocityX();
			sendUnit->vely = (short)_unit->GetVelocityY();
			sendUnit->shareID = (unsigned int)_unit->GetSharedID();

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, sendUnit, sizeof(CUpdateUnitPacket));
			m_UDPSend.Send(buf, sizeof(CUpdateUnitPacket), *m_ConnectedIP);
			delete sendUnit;
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendUnitStatUpdatePacket(CUnitObject* _unit)
{
	try
	{
		if(IsConnected())
		{
			CUpdateUnitStatsPacket* packet = new CUpdateUnitStatsPacket;
			packet->m_UpdateType = NW_UPDATESTATS;
			packet->shareID = (unsigned int)_unit->GetSharedID();
			packet->m_MessageID = m_RetryMessageTotal++;
			packet->mine = _unit->GetNWOwner();
			packet->attack = _unit->GetBaseAttack();
			packet->defense = _unit->GetBaseDefense();
			packet->evade = _unit->GetBaseEvade();
			packet->movementSpeed = _unit->GetBaseMovement();
			packet->attackSpeed = _unit->GetBaseAttackSpeed();
			

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CUpdateUnitStatsPacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CUpdateUnitStatsPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendStatusEffectPacket(CUnitObject* unit, short timer, short b, short u, short e, short bs, short us, short es)
{
	try
	{
		if(IsConnected())
		{
			CSendStatusEffectPacket* packet = new CSendStatusEffectPacket;
			packet->m_UpdateType = NW_STATUSEFFECT;
			packet->shareID = (unsigned int)unit->GetSharedID();
			packet->m_MessageID = m_RetryMessageTotal++;
			packet->mine = unit->GetNWOwner();


			packet->Begin = b;
			packet->BeginSender = bs;
			packet->End = e;
			packet->EndSender = es;
			packet->Update = u;
			packet->UpdateSender = us;
			packet->Timer = timer;

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CSendStatusEffectPacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CSendStatusEffectPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

/*
void CNetworkManager::SendModUnitStatsPacket(CModUnitStatsPacket _packet)
{
	try
	{
		if(IsConnected())
		{
			CModUnitStatsPacket* packet = new CModUnitStatsPacket;
			memcpy(packet, &_packet, sizeof(CModUnitStatsPacket));
			packet->m_UpdateType = NW_MODSTATS;
			packet->shareID = (unsigned int)_unit->GetSharedID();
			packet->m_MessageID = m_RetryMessageTotal++;
			
			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CModUnitStatsPacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CModUnitStatsPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}*/

void CNetworkManager::SendUseSimpleSkillPacket(CUnitObject* _unit, unsigned char _skillID)
{
	try
	{
		if(IsConnected())
		{
			CUseSimpleSkillPacket* packet = new CUseSimpleSkillPacket;
			packet->m_UpdateType = NW_USESIMPLESKILL;
			packet->shareID = (unsigned int)_unit->GetSharedID();
			packet->m_MessageID = m_RetryMessageTotal++;
			packet->mine = _unit->GetNWOwner();
			packet->skillNum = _skillID;
			

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CUseSimpleSkillPacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CUseSimpleSkillPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendDropResourcePacket(int _rscType, unsigned int _sharedID, bool _mine)
{
	try
	{
		if(IsConnected())
		{
			CDropResourcePacket* packet = new CDropResourcePacket;
			packet->m_UpdateType = NW_DROPRESOURCE;
			packet->unitID = _sharedID;
			packet->mine = _mine;
			packet->resourceType = _rscType;
			//ID for validation
			packet->m_MessageID = m_RetryMessageTotal++;

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CDropResourcePacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CDropResourcePacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendDealDamagePacket(unsigned int _from, unsigned int _to, unsigned int _amt)
{
	try
	{
		if(IsConnected())
		{
			CDealDamagePacket* packet = new CDealDamagePacket;
			packet->m_UpdateType = NW_DAMAGE;
			//ID for validation
			packet->m_MessageID = m_RetryMessageTotal++;

			packet->from = _from;
			packet->to = _to;
			packet->amt = _amt;

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CDealDamagePacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CDealDamagePacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendMoveCommandPacket(CMoveCommand* _command, unsigned int _id)
{
	try
	{
		if(IsConnected())
		{
			CMoveCommandPacket* packet = new CMoveCommandPacket;
			packet->m_UpdateType = NW_MOVECOMMAND;
			//ID for validation
			packet->m_MessageID = m_RetryMessageTotal++;
			packet->shareID = (unsigned int)_id;
			packet->partAttacking = _command->GetPartAttacking();
			packet->numPaths = (short)_command->GetPath()->size();
			int i = 0;
			for(std::list<CPathNode*>::iterator Iterator = _command->GetPath()->begin(); Iterator != _command->GetPath()->end(); ++Iterator)
			{
				packet->pathID[i] = (short)(*Iterator)->GetNodeID();
				i++;
			}
			packet->posx = (short)_command->GetDestinationX();
			packet->posy = (short)_command->GetDestinationY();

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CMoveCommandPacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CMoveCommandPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}


void CNetworkManager::SendAttackCommandPacket(unsigned int _from, unsigned int _to)
{
	try
	{
		if(IsConnected())
		{
			CAttackCommandPacket* packet = new CAttackCommandPacket;
			packet->m_UpdateType = NW_ATTACKCOMMAND;
			//ID for validation
			packet->m_MessageID = m_RetryMessageTotal++;

			packet->shareID = _from;
			packet->attackID = _to;

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CAttackCommandPacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CAttackCommandPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendCreateProjectilePacket(std::string creationString, short hitFunction, float speed, short damage, float x, float y, unsigned int id, bool mine)
{
	try
	{
		if(IsConnected())
		{
			CCreateProjectilePacket* packet = new CCreateProjectilePacket;
			packet->m_UpdateType = NW_CREATEPROJECTILE;
			//ID for validation
			packet->m_MessageID = m_RetryMessageTotal++;
			packet->shareID = id;
			packet->mine = mine;
			packet->speed = speed;
			packet->damage = damage;
			packet->x = x;
			packet->y = y;
			packet->hitFunction = hitFunction;
			memcpy(packet->creationString, creationString.c_str(), creationString.size()+1);

			//Send
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CCreateProjectilePacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CCreateProjectilePacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendTextMessage(char* text)
{
	try
	{
		if(IsConnected())
		{
			CTextMessageNetworkUpdate* sendText = new CTextMessageNetworkUpdate;
			sendText->m_UpdateType = NW_TEXTMESSAGE;
			//Send an ID for retry and validation purposes
			sendText->m_MessageID = m_RetryMessageTotal++;
			strcpy_s(sendText->text, 256, text);
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, &sendText, sizeof(CTextMessageNetworkUpdate));
			m_RetryList.push_back(sendText);
			m_UDPSend.Send(buf, PACKET_SIZE, *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendConnectPacket(char* IP, char* name)
{
	try
	{
		if(IsConnected())
		{
			CConnectPacket* packet = new CConnectPacket;
			packet->m_UpdateType = NW_CONNECT;
			//Send an ID for retry and validation purposes
			packet->m_MessageID = m_RetryMessageTotal++;
			strcpy_s(packet->szMyIP, 24, IP);
			strcpy_s(packet->szMyName, 32, name);
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CConnectPacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CConnectPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() << std::endl;
	}
}

void CNetworkManager::SendDisconnectPacket()
{
	try
	{
		if(IsConnected())
		{
			CDisconnectPacket* packet = new CDisconnectPacket;
			packet->m_UpdateType = NW_DISCONNECT;
			//Send an ID for retry and validation purposes
			packet->m_MessageID = m_RetryMessageTotal++;


			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, packet, sizeof(CDisconnectPacket));
			m_RetryList.push_back(packet);
			m_UDPSend.Send(buf, sizeof(CDisconnectPacket), *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::SendValidateMessage(int _validateid)
{
	try
	{
		if(IsConnected())
		{
			CValidateMessage sendValidate;
			sendValidate.m_MessageID = 0;
			sendValidate.m_UpdateType = NW_VALIDATE;
			sendValidate.m_ValidateID = _validateid;
			sckt::byte buf[PACKET_SIZE];
			memcpy(buf, &sendValidate, PACKET_SIZE);
			m_UDPSend.Send(buf, PACKET_SIZE, *m_ConnectedIP);
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}


void CNetworkManager::RetryMessage(int id)
{
	try
	{
		if(IsConnected())
		{
			sckt::byte buf[PACKET_SIZE] = {0};
			memset ( buf, 0, PACKET_SIZE );
			INetworkUpdate baseMessage;
			memcpy(&baseMessage, m_RetryList[id], sizeof(baseMessage));

			//For the different types of messages that need to be sent
			if(baseMessage.m_UpdateType == NW_TEXTMESSAGE)
			{
				CTextMessageNetworkUpdate textMessage;
				memcpy(&textMessage, m_RetryList[id], sizeof(textMessage));
				memcpy(buf, &textMessage, sizeof(textMessage));
				m_UDPSend.Send(buf, sizeof(textMessage), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_MOVECOMMAND)
			{
				CMoveCommandPacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_ATTACKCOMMAND)
			{
				CAttackCommandPacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, PACKET_SIZE, *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_CONNECT)
			{
				CConnectPacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_DAMAGE)
			{
				CDealDamagePacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_DROPRESOURCE)
			{
				CDropResourcePacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_DISCONNECT)
			{
				CDisconnectPacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_UPDATESTATS)
			{
				CUpdateUnitStatsPacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_USESIMPLESKILL)
			{
				CUseSimpleSkillPacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_STATUSEFFECT)
			{
				CSendStatusEffectPacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
			else if(baseMessage.m_UpdateType == NW_CREATEPROJECTILE)
			{
				CCreateProjectilePacket packet;
				memcpy(&packet, m_RetryList[id], sizeof(packet));
				memcpy(buf, &packet, sizeof(packet));
				m_UDPSend.Send(buf, sizeof(packet), *m_ConnectedIP);
			}
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

void CNetworkManager::Update(float dt)
{
	m_RetryTimer += dt;
	if(m_RetryTimer > m_MaxRetry)
	{
		//Resend old messages
		if(m_RetryList.size())
		{
			for(unsigned int i = 0; i < m_RetryList.size(); i++)
			{
				RetryMessage(i);
			}
		}
		m_RetryTimer = m_MaxRetry;
	}
}

INetworkUpdate* CNetworkManager::ReturnPacket(ENetworkUpdateType _type)
{
	for(std::list<INetworkUpdate*>::iterator Iterator = m_WaitingPackets.begin(); Iterator != m_WaitingPackets.end(); ++Iterator)
	{
		if((*Iterator)->m_UpdateType == _type)
		{
			INetworkUpdate* ret = (*Iterator);
			m_WaitingPackets.erase(Iterator);
			return ret;	
		}
	}
	return NULL;
}

void CNetworkManager::Receive()
{
	try
	{
		INetworkUpdate baseMessage;
		sckt::byte buf[PACKET_SIZE];
		
		while(m_UDPListen.Recv(buf, PACKET_SIZE, *m_ConnectedIP) != 0)
		{
			memcpy(&baseMessage, buf, sizeof(baseMessage));

			switch(baseMessage.m_UpdateType)
			{
			case NW_TEXTMESSAGE:
				CTextMessageNetworkUpdate textMessage;
				memcpy(&textMessage, buf, sizeof(textMessage));
				
				//If we didnt get this packet already
				if(!CheckReceived(textMessage.m_MessageID))
				{
					//Respond to the text message we just got
					SNewText newText;
					strcpy_s(newText.text, 256, textMessage.text);
					m_NewMessages.push_back(newText);
					m_ReceiveList.push_back(textMessage.m_MessageID);
				}

				//Always remind the other computer we got it, you dummy
				SendValidateMessage(textMessage.m_MessageID);
				break;
			case NW_CONNECT:
				{
					CConnectPacket* packet = new CConnectPacket;
					memcpy(packet, buf, sizeof(CConnectPacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						//Respond to the text message we just got
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_VALIDATE:
				CValidateMessage validateMessage;
				memcpy(&validateMessage, buf, sizeof(validateMessage));

				
				for(unsigned int i = 0; i < m_RetryList.size(); i++)
				{
					if(m_RetryList[i]->m_MessageID == validateMessage.m_ValidateID)
					{
						delete m_RetryList[i];
						m_RetryList.erase(m_RetryList.begin()+i);
						break;
					}
				}
				break;
			case NW_CREATEUNIT:
				{
					CCreateUnitPacket* packet = new CCreateUnitPacket;
					memcpy(packet, buf, sizeof(CCreateUnitPacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						//Respond to the create message we just got
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_UPDATEUNIT:
				{
					CUpdateUnitPacket* packet = new CUpdateUnitPacket;
					memcpy(packet, buf, sizeof(CUpdateUnitPacket));
					
					//Respond to the update message we just got
					m_WaitingPackets.push_back(packet);
				}
				break;
			case NW_ATTACKCOMMAND:
				{
					CAttackCommandPacket* packet = new CAttackCommandPacket;
					memcpy(packet, buf, sizeof(CAttackCommandPacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						//Respond to the create message we just got
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_MOVECOMMAND:
				{
					CMoveCommandPacket* packet = new CMoveCommandPacket;
					memcpy(packet, buf, sizeof(CMoveCommandPacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						//Respond to the create message we just got
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_DAMAGE:
				{
					CDealDamagePacket* packet = new CDealDamagePacket;
					memcpy(packet, buf, sizeof(CDealDamagePacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						//Respond to the create message we just got
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_DROPRESOURCE:
				{
					CDropResourcePacket* packet = new CDropResourcePacket;
					memcpy(packet, buf, sizeof(CDropResourcePacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						//Respond to the create message we just got
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_DISCONNECT:
				{
					CDisconnectPacket* packet = new CDisconnectPacket;
					memcpy(packet, buf, sizeof(CDisconnectPacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						//Respond to the create message we just got
						for(unsigned int i = 0; i < m_RetryList.size(); i++)
						{
							delete m_RetryList[i];
						}
						m_RetryList.clear();
						if(m_ConnectedIP)
							delete m_ConnectedIP;
						m_ConnectedIP = NULL;
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_UPDATESTATS:
				{
					CUpdateUnitStatsPacket* packet = new CUpdateUnitStatsPacket;
					memcpy(packet, buf, sizeof(CUpdateUnitStatsPacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_USESIMPLESKILL:
				{
					CUseSimpleSkillPacket* packet = new CUseSimpleSkillPacket;
					memcpy(packet, buf, sizeof(CUseSimpleSkillPacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_STATUSEFFECT:
				{
					CSendStatusEffectPacket* packet = new CSendStatusEffectPacket;
					memcpy(packet, buf, sizeof(CSendStatusEffectPacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			case NW_CREATEPROJECTILE:
				{
					CCreateProjectilePacket* packet = new CCreateProjectilePacket;
					memcpy(packet, buf, sizeof(CCreateProjectilePacket));
					
					//If we didnt get this packet already
					if(!CheckReceived(packet->m_MessageID))
					{
						m_WaitingPackets.push_back(packet);
						m_ReceiveList.push_back(packet->m_MessageID);
					}

					//Always remind the other computer we got it, you dummy
					SendValidateMessage(packet->m_MessageID);
				}
				break;
			}
		}
	}
	catch(sckt::Exc &e)
	{
		std::cout << "Network error: "<< e.What() <<std::endl;
	}
}

bool CNetworkManager::CheckReceived(unsigned int id)
{
	for(unsigned int i = 0; i < m_ReceiveList.size(); i++)
	{
		if(id == m_ReceiveList[i])
			return true;
	}
	return false;
}

void CNetworkManager::IterateMessages(FunctionPointer Function)
{
	for(unsigned int i = 0; i < m_NewMessages.size(); i++)
	{
		Function(m_NewMessages[i].text);
	}
	m_NewMessages.clear();
}