//////////////////////////////////////////////////////
// File: "CTile.h"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/18/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CTILE_H_
#define CTILE_H_

class CTile
{
private:
	int m_nTileID;
	int m_nCollisionID;

public:
	///////////////constructor and destructor///////////////
	CTile(void)		{ m_nTileID = -1; m_nCollisionID = -1; }
	~CTile(void)	{}


	/////////////////accessors and mutators/////////////////
	int GetTileID(void) const				{ return m_nTileID; }
	void SetTileID(int nTileID)				{ m_nTileID = nTileID; }

	int GetCollisionID(void) const			{ return m_nCollisionID; }
	void SetCollisionID(int nCollisionID)	{ m_nCollisionID = nCollisionID; }
};
#endif