#ifndef INETWORKABLE_H_
#define INETWORKABLE_H_


class INetworkable
{
protected:
	unsigned int m_ShareID;
	unsigned int m_Revision;
	bool m_bMine;

public:
	INetworkable() { m_bMine = true; }
	virtual void UpdateNetworked(float dt){};
	virtual void SendUpdatePacket(){};

	void SetSharedID(unsigned int _id) {m_ShareID = _id;}
	void SetNWOwner(bool _mine) { m_bMine = _mine; }

	unsigned int GetSharedID() { return m_ShareID; }
	bool GetNWOwner() { return m_bMine; }
};

#endif