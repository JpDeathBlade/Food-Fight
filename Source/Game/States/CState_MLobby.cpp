#include "CState_MLobby.h"
#include "CState_Gameplay.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Managers/Bad Chef/Networking/CNetworkManager.h"
#include "../../Managers/Bad Chef/Networking/NetworkUpdateClasses.h"
#include "../../CGame.h"
#include "../Objects/GUI Objects/CButton.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

CState_MLobby::CState_MLobby()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	SetSprite(ButtonSprite);
	ChallengeButtonOnClick.SetLobby(this);
	AcceptButtonOnClick.SetLobby(this);
	DeclineButtonOnClick.SetLobby(this);

	for(unsigned char i = 0; i < 2; ++i)
	{
		m_Buttons[i] = new CButton();
		m_Buttons[i]->SetSprite(ButtonSprite);
		m_Buttons[i]->SetPosition(110.0f, 160.0f);
		m_Buttons[i]->SetFontFileName("Resources/Data/Font.spr");
		m_Buttons[i]->SetFontScale(0.75f, 0.75f);
		m_Buttons[i]->SetStringOffset(10, 10);
	}

	m_Buttons[0]->SetString(STRING->FindString("Challenge").c_str());
	m_Buttons[0]->SetOnRelease(&ChallengeButtonOnClick);

	m_Buttons[1]->SetString(STRING->FindString("Back").c_str());
	m_Buttons[1]->SetOnRelease(&BackButtonOnClick);

	for(unsigned char i = 0; i < 2; ++i)
	{
		m_Buttons[i]->FitToText();
		if(i > 0)
			m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
		m_ObjectManager.AddObjectToLayer(0, m_Buttons[i]);
	}

	m_nSelection = static_cast<EMenu>(0);
	SetSelection(MIN);

	strcpy_s(m_TypedIP, 24, "Type an IP here\0");
	m_IsTypingIP = false;
	m_SubState = START;
}

CState_MLobby::~CState_MLobby()
{

}

void CState_MLobby::OnEnter()
{
	char hostname[80];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		strcpy_s(m_MyIP, 24, STRING->FindString("Not connected to a network!").c_str());
	}

	hostent *hostLookup = gethostbyname(hostname);
	if(hostLookup == 0)
	{
		strcpy_s(m_MyIP, 24, STRING->FindString("Not connected to a network!").c_str());
	}

	for (int i = 0; hostLookup->h_addr_list[i] != 0; ++i) {
		in_addr addr;
		memcpy(&addr, hostLookup->h_addr_list[i], sizeof(struct in_addr));
		strcpy_s(m_MyIP, 24, inet_ntoa(addr));
	}
}

bool CState_MLobby::Input()
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[UP]) || XBOX->GetController(0)->IsPressed_DPAD_UP())
	{
		SetSelection(static_cast<EMenu>(GetSelection() - 1));
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface")->Play();
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[DOWN]) || XBOX->GetController(0)->IsPressed_DPAD_DOWN())
	{
		SetSelection(static_cast<EMenu>(GetSelection() + 1));
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface")->Play();
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || XBOX->GetController(0)->IsPressed_A())
	{
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface")->Play();
		if((m_Buttons[GetSelection()]->GetOnRelease()))
			(*(m_Buttons[GetSelection()]->GetOnRelease()))();
	}

	if(m_IsTypingIP)
	{
		char newChar = CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx();
		if(((newChar >= 48 && newChar <= 57) || newChar == 8 || newChar == 46))
		{
			if(strcmp(m_TypedIP, "Type an IP here") == 0)
			{
				if(newChar == 8)
				{
					char buffer[2];
					buffer[0] = NULL;
					buffer[1] = NULL;
					strcpy_s(m_TypedIP, buffer);
				}
				else
				{
					char buffer[2];
					buffer[0] = newChar;
					buffer[1] = NULL;
					strcpy_s(m_TypedIP, buffer);
				}
			}
			else if(newChar == 8)
			{
				int length = strlen(m_TypedIP);
				if(length > 0)
				{
					m_TypedIP[length-1] = NULL;
				}
			}
			else if((strlen(m_TypedIP) < 15))
			{
				int length = strlen(m_TypedIP);
				m_TypedIP[length] = newChar;
				m_TypedIP[length+1] = NULL;
			}
		}
	}

	return true;
}

void CState_MLobby::Update(float _delta)
{
	NW_MANAGER_START
	CNetworkManager::GetInstance()->Update(_delta);
	NW_MANAGER_END
	CConnectPacket* ConnectPacket = NULL;
	CDisconnectPacket* DisconnectPacket = NULL;

	if(m_SubState == START)
	{
		NW_MANAGER_START
			if(ConnectPacket = static_cast<CConnectPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_CONNECT)))
			{
				//Save challenger info
				CNetworkManager::GetInstance()->SetConnectedIP(ConnectPacket->szMyIP);
				strcpy_s(m_Challenger, 32, ConnectPacket->szMyName);
				delete ConnectPacket;
				m_SubState = CHALLENGED;

				m_Buttons[0]->SetString(STRING->FindString("Accept").c_str());
				m_Buttons[0]->SetOnRelease(&AcceptButtonOnClick);

				m_Buttons[1]->SetString(STRING->FindString("Decline").c_str());
				m_Buttons[1]->SetOnRelease(&DeclineButtonOnClick);

				for(unsigned char i = 0; i < 2; ++i)
				{
					m_Buttons[i]->FitToText();
					if(i > 0)
						m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
				}
			}
			NW_MANAGER_END
	}
	else if(m_SubState == CHALLENGING)
	{
		//Wait for connect or decline packet
		NW_MANAGER_START
			if(ConnectPacket = static_cast<CConnectPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_CONNECT)))
			{
				//Is this who we challenged?
				if(strcmp(m_TypedIP, ConnectPacket->szMyIP) == 0)
				{
					//Start game as first player
					CGame::GetInstance()->ChangeState(new CState_Gameplay(1, true));
				}
				delete ConnectPacket;
			}
			NW_MANAGER_END
	}

	NW_MANAGER_START
	if(DisconnectPacket = static_cast<CDisconnectPacket*>(CNetworkManager::GetInstance()->ReturnPacket(NW_DISCONNECT)))
	{
		CNetworkManager::GetInstance()->SetConnectedIP(NULL);
		m_SubState = START;

		m_Buttons[0]->SetString(STRING->FindString("Challenge").c_str());
		m_Buttons[0]->SetOnRelease(&ChallengeButtonOnClick);

		m_Buttons[1]->SetString(STRING->FindString("Back").c_str());
		m_Buttons[1]->SetOnRelease(&BackButtonOnClick);

		for(unsigned char i = 0; i < 2; ++i)
		{
			m_Buttons[i]->FitToText();
			if(i > 0)
				m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
		}

		delete DisconnectPacket;
	}
	NW_MANAGER_END
}

void CState_MLobby::Draw()
{
	CSGD_Direct3D::GetInstance()->Clear();

	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/FoodFight_Menu.png")->Draw(D3DXVECTOR2(0.0f, -250.0f));

	DrawGUIFrame(this, D3DXVECTOR2(100, 50), D3DXVECTOR2(1024 - 200, 768 - 100));

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);

	switch(m_SubState)
	{
	case START:
		//Draw title
		BITMAP->DrawStringSprite("Resources/Data/Font.spr",STRING->FindString("Multiplayer").c_str(),512.0f, 100.0f);
		BITMAP->DrawStringSprite("Resources/Data/Font.spr", m_TypedIP, m_Buttons[0]->GetPositionX() + m_Buttons[0]->GetSizeX() + 25, m_Buttons[0]->GetPositionY(), 1.0, 1.0, 0, m_IsTypingIP ? D3DCOLOR_XRGB(255, 127, 127) : D3DCOLOR_XRGB(255, 255, 255));
		break;
	case CHALLENGED:
		//Draw title
		char title[128];
		sprintf_s(title, 128, "%s %s!", STRING->FindString("Challege from").c_str(), m_Challenger);
		BITMAP->DrawStringSprite("Resources/Data/Font.spr",title,512.0f, 100.0f);
		break;
	case CHALLENGING:
		//Draw title
		BITMAP->DrawStringSprite("Resources/Data/Font.spr",STRING->FindString("Waiting...").c_str(),512.0f, 100.0f);
		break;
	}

	//Draw My IP
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", "Your IP:", 164.0f, 640.0f, 0.8f, 0.8f);
	BITMAP->DrawStringSprite("Resources/Data/Font.spr",m_MyIP,300.00f,640.0f, 0.8f, 0.8f);
}

void CState_MLobby::CChallengeButtonOnClickFunctor::operator()()
{
	if(GetLobby()->m_IsTypingIP)
	{
		NW_MANAGER_START
		CNetworkManager::GetInstance()->SetConnectedIP(GetLobby()->m_TypedIP);
/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BUG-007
// BUG FIX START
/////////////////////////////////////////////////////////////////
		if(CNetworkManager::GetInstance()->IsConnected())
		{
			CNetworkManager::GetInstance()->SendConnectPacket(GetLobby()->m_MyIP, CProfileManager::GetInstance()->GetCurrentProfile()->szName);
		}
///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BUG-007
//////////////////////////////////////////////////////////////////////////
		NW_MANAGER_END

		if(CNetworkManager::GetInstance()->IsConnected())
		{
			GetLobby()->m_SubState = CHALLENGING;

			GetLobby()->m_Buttons[0]->SetString(STRING->FindString("Wait").c_str());
			GetLobby()->m_Buttons[0]->SetOnRelease(NULL);
			GetLobby()->m_Buttons[0]->Disable();

			GetLobby()->m_Buttons[1]->SetString(STRING->FindString("Cancel").c_str());
			GetLobby()->m_Buttons[1]->SetOnRelease(&GetLobby()->DeclineButtonOnClick);

			for(unsigned char i = 0; i < 2; ++i)
			{
				GetLobby()->m_Buttons[i]->FitToText();
				if(i > 0)
					GetLobby()->m_Buttons[i]->SetPositionY(GetLobby()->m_Buttons[i - 1]->GetPositionY() + GetLobby()->m_Buttons[i - 1]->GetSizeY() + 10);
			}
		}
	}
	GetLobby()->m_IsTypingIP = !GetLobby()->m_IsTypingIP;
}

CState_MLobby* CState_MLobby::CChallengeButtonOnClickFunctor::GetLobby() const
{
	return m_Lobby;
}

void CState_MLobby::CChallengeButtonOnClickFunctor::SetLobby(CState_MLobby* Lobby)
{
	m_Lobby = Lobby;
}

void CState_MLobby::CAcceptButtonOnClickFunctor::operator()()
{
	NW_MANAGER_START
	CNetworkManager::GetInstance()->SendConnectPacket(GetLobby()->m_MyIP, CProfileManager::GetInstance()->GetCurrentProfile()->szName);
	NW_MANAGER_END
	CGame::GetInstance()->ChangeState(new CState_Gameplay(2, true));
}

CState_MLobby* CState_MLobby::CAcceptButtonOnClickFunctor::GetLobby() const
{
	return m_Lobby;
}

void CState_MLobby::CAcceptButtonOnClickFunctor::SetLobby(CState_MLobby* Lobby)
{
	m_Lobby = Lobby;
}

void CState_MLobby::CBackButtonOnClickFunctor::operator()()
{
	CGame::GetInstance()->PopState();
}

void CState_MLobby::CDeclineButtonOnClickFunctor::operator()()
{
	NW_MANAGER_START
	CNetworkManager::GetInstance()->SendDisconnectPacket();
	CNetworkManager::GetInstance()->SetConnectedIP(NULL);
	NW_MANAGER_END
	GetLobby()->m_SubState = START;

	GetLobby()->m_Buttons[0]->SetString(STRING->FindString("Challenge").c_str());
	GetLobby()->m_Buttons[0]->SetOnRelease(&GetLobby()->ChallengeButtonOnClick);
	GetLobby()->m_Buttons[0]->Enable();

	GetLobby()->m_Buttons[1]->SetString(STRING->FindString("Back").c_str());
	GetLobby()->m_Buttons[1]->SetOnRelease(&GetLobby()->BackButtonOnClick);

	for(unsigned char i = 0; i < 2; ++i)
	{
		GetLobby()->m_Buttons[i]->FitToText();
		if(i > 0)
			GetLobby()->m_Buttons[i]->SetPositionY(GetLobby()->m_Buttons[i - 1]->GetPositionY() + GetLobby()->m_Buttons[i - 1]->GetSizeY() + 10);
	}
}

CState_MLobby* CState_MLobby::CDeclineButtonOnClickFunctor::GetLobby() const
{
	return m_Lobby;
}

void CState_MLobby::CDeclineButtonOnClickFunctor::SetLobby(CState_MLobby* Lobby)
{
	m_Lobby = Lobby;
}
CState_MLobby::EMenu CState_MLobby::GetSelection() const
{
	return m_nSelection;
}

void CState_MLobby::SetSelection(CState_MLobby::EMenu Selection)
{
	if(m_Buttons[m_nSelection])
		m_Buttons[m_nSelection]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));

	if(Selection > MAX)
		Selection = MIN;
	else if(Selection < MIN)
		Selection = MAX;

	m_nSelection = Selection;

	if(m_Buttons[m_nSelection])
		m_Buttons[m_nSelection]->SetTextColor(D3DCOLOR_XRGB(255, 127, 127));
}
