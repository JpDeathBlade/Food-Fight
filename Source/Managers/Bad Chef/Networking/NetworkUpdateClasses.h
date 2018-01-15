#ifndef NETWORKUPDATE_H_
#define NETWORKUPDATE_H_

#include "../../../Game/Objects/Units/CStatusEffect.h"

enum ENetworkUpdateType { NW_TEXTMESSAGE, NW_VALIDATE, NW_MOVECOMMAND, NW_ATTACKCOMMAND, NW_CONNECT, NW_CREATEUNIT,
						NW_UPDATEUNIT, NW_DAMAGE, NW_DROPRESOURCE, NW_DISCONNECT, NW_UPDATESTATS, NW_USESIMPLESKILL, NW_MODSTATS, NW_STATUSEFFECT, NW_CREATEPROJECTILE };


class INetworkUpdate
{
public:
	ENetworkUpdateType m_UpdateType;
	unsigned int m_MessageID;
};

class CTextMessageNetworkUpdate : public INetworkUpdate
{
public:
	char text[256];
};

class CConnectPacket : public INetworkUpdate
{
public:
	char szMyName[32];
	char szMyIP[24];
};

class CValidateMessage : public INetworkUpdate
{
public:
	unsigned int m_ValidateID;
};

class CCreateUnitPacket : public INetworkUpdate
{
public:
	short posx;
	short posy;
	char createString[32];
	unsigned int shareID;
	unsigned short	defense;
	unsigned short	attack;
	float			attackSpeed;
	float			evade;
	unsigned short	movementSpeed;

};

class CUpdateUnitPacket : public INetworkUpdate
{
public:
	unsigned int shareID;
	short posx;
	short posy;
	short velx;
	short vely;
};

class CMoveCommandPacket  : public INetworkUpdate
{
public:
	unsigned int shareID;
	bool partAttacking;
	short posx;
	short posy;
	short numPaths;
	short pathID[64];
};

class CAttackCommandPacket : public INetworkUpdate
{
public:
	unsigned int shareID;
	unsigned int attackID;
};

class CDealDamagePacket : public INetworkUpdate
{
public:
	unsigned int to;
	unsigned int from;
	short amt;
};

class CDropResourcePacket : public INetworkUpdate
{
public:
	int resourceType;
	unsigned int unitID;
	bool mine;
};

class CDisconnectPacket : public INetworkUpdate
{
public:

};

class CUpdateUnitStatsPacket : public INetworkUpdate
{
public:
	unsigned int shareID;
	bool mine;
	unsigned short	defense;
	unsigned short	attack;
	float			attackSpeed;
	float			evade;
	unsigned short	movementSpeed;

};

class CUseSimpleSkillPacket : public INetworkUpdate
{
public:
	unsigned int shareID;
	bool mine;
	unsigned char skillNum;
};

class CModUnitStatsPacket : public INetworkUpdate
{
public:
	CModUnitStatsPacket()
	{
		m_fAttackMultiply = 0;
		m_fDefenseMultiply = 0;
		m_fMovementMultiply = 0;
		m_fEvadeMultiply = 0;
		m_fAttackAdd = 0;
		m_fDefenseAdd = 0;
		m_fMovementAdd = 0;
		m_fEvadeAdd = 0;
	}
	unsigned int shareID;
	bool mine;
	//Modifications to stats
	//Multiplying
	float m_fAttackMultiply;
	float m_fDefenseMultiply;
	float m_fMovementMultiply;
	float m_fEvadeMultiply;
	//Adding
	float m_fAttackAdd;
	float m_fDefenseAdd;
	float m_fMovementAdd;
	float m_fEvadeAdd;
};

class CSendStatusEffectPacket : public INetworkUpdate
{
public:
	unsigned int shareID;
	bool mine;	
	short Begin;
	short Update;
	short End;
	short BeginSender;
	short UpdateSender;
	short EndSender;
	short Timer;
};

class CCreateProjectilePacket : public INetworkUpdate
{
public:
	unsigned int shareID;
	bool mine;	
	char creationString[64];
	float speed;
	short damage;
	float x;
	float y;
	short hitFunction;
};

#endif