#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#define PORT_NUMBER 13379
#define PACKET_SIZE 1024

#define NW_MANAGER_START CNetworkManager::Lock();
#define NW_MANAGER_END CNetworkManager::Unlock();

#pragma comment(lib, "sckt Library.lib")
#pragma comment(lib, "Kernel32.lib")
#include "../../../../../sckt Library/sckt.hpp"
#include "NetworkUpdateClasses.h"

#include <vector>
#include <list>
#include <windows.h>

class INetworkable;
class CUnitObject;
class CMoveCommand;


struct SNewText
{
	char text[256];
};

class CNetworkManager
{
private:
	volatile static CNetworkManager* m_Instance;

	static LPCRITICAL_SECTION m_pCS;

	CNetworkManager(); // Default Constructor
	CNetworkManager(const CNetworkManager& Source); // Copy Constructor
	CNetworkManager& operator=(const CNetworkManager& Source); // Assignment Operator
	~CNetworkManager(); // Destructor

	//For shutting down the Receive thread
	bool running;

	//Sockets/IPs/NetworkStuff
	sckt::UDPSocket m_UDPListen;
	sckt::UDPSocket m_UDPSend;
	sckt::IPAddress* m_ConnectedIP;
	sckt::Library socketsLib;

	//Pointers to game objects
	std::vector<INetworkable*> m_SharedList;

	//Packets waiting to be read
	std::list<INetworkUpdate*> m_WaitingPackets;

	//Waiting Messages
	typedef void(*FunctionPointer)(char*);
	std::vector<SNewText> m_NewMessages;

	//Packet Synchronization
	int m_SharedIDTotal;
	int m_RetryMessageTotal;
	float m_RetryTimer;
	float m_MaxRetry;
	std::vector<INetworkUpdate*> m_RetryList;
	std::vector<unsigned int> m_ReceiveList;
public:
	void SetConnectedIP(char* _IP);
	bool CheckValidIP(){}

	void SendTextMessage(char* text);
	void SendValidateMessage(int _validateid);
	void SendConnectPacket(char* IP, char* name);
	void SendCreateUnitPacket(CUnitObject* _unit, char* _str);
	void SendUnitUpdatePacket(CUnitObject* _unit);
	void SendUnitStatUpdatePacket(CUnitObject* _unit);
	void SendUseSimpleSkillPacket(CUnitObject* _unit, unsigned char _skillID);
	void SendMoveCommandPacket(CMoveCommand* _command, unsigned int _id);
	void SendAttackCommandPacket(unsigned int _from, unsigned int _to);
	void SendDealDamagePacket(unsigned int _from, unsigned int _to, unsigned int _amt);
	void SendDropResourcePacket(int _rscType, unsigned int _sharedID, bool _mine);
	void SendStatusEffectPacket(CUnitObject* unit, short timer, short b, short u, short e, short bs, short us, short es);
	void SendCreateProjectilePacket(std::string creationString, short hitFunction, float speed, short damage, float x, float y, unsigned int id, bool mine);
	void SendDisconnectPacket();

	void Update(float dt);
	void Receive();
	void RetryMessage(int id);
	bool CheckReceived(unsigned int id);

	INetworkUpdate* ReturnPacket(ENetworkUpdateType _type);

	void IterateMessages(FunctionPointer Function);
	
	static CNetworkManager* GetInstance();
	static void DeleteInstance();
	static void Lock();
	static void Unlock();

	void Shutdown();
	bool CheckRunning();

	bool IsConnected();

	void static SetCS(LPCRITICAL_SECTION _CS) { m_pCS = _CS; }
};


#endif