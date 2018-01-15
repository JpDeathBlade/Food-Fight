//////////////////////////////////////////////////////
// File: "CTileStage.h"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/18/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#ifndef CTILESTAGE_H_
#define CTILESTAGE_H_

#include "../Base/IObject.h"
#include <vector>

class CTile;
class CObjectManager;
class CStove;
class CPlate;

class CTileStage : public IObject
{
private:
	static	CTileStage*	m_pInstance;

	char*				m_szName;
	CTile**				m_arrTiles;
	int					m_nMapWidth;
	int					m_nMapHeight;
	int					m_nTileWidth;
	int					m_nTileHeight;
	int					m_nTilesetWidth;
	IDirect3DTexture9*	m_texTileSet;
	std::vector<CStove*> m_Stoves;
	std::vector<CPlate*> m_Plates;

	//Constructor and destructor
	CTileStage(void);
	~CTileStage(void);

	//Disabling the assignment operator and copy constructor
	CTileStage(const CTileStage& obj) {}
	CTileStage& operator = (const CTileStage& obj) { return *this; }

	///////////////////////////////////////////////////////////////
	// Function: CellAlgorithm
	//
	// In: An int for the id of the tile.
	//
	// Out: A RECT that is the source to draw from.
	//
	// Purpose: To know where to draw the tile from based on its ID.
	///////////////////////////////////////////////////////////////
	RECT CellAlgorithm(int nID);

	///////////////////////////////////////////////////////////////
	// Function: TilesetCellAlgorithm
	//
	// In: An int for the id of the tile.
	//
	// Out: A RECT that is the source to draw from.
	//
	// Purpose: To know where to draw the tile from based on its ID.
	///////////////////////////////////////////////////////////////
	RECT TilesetCellAlgorithm(int nID);

public:
	///////////////////////////////////////////////////////////////
	// Function: GetInstance
	//
	// In: void
	//
	// Out: CTileStage pointer that is the singleton instance of CTileStage.
	//
	// Purpose: Returns the singleton pointer.
	///////////////////////////////////////////////////////////////
	static CTileStage* GetInstance(void);

	///////////////////////////////////////////////////////////////
	// Function: Update
	//
	// In: A float for the amount of time that has elapsed since the
	//		last update.
	//
	// Out: void
	//
	// Purpose: Updates the TileMap based on time.
	///////////////////////////////////////////////////////////////
	void Update(float Delta); 

	///////////////////////////////////////////////////////////////
	// Function: OnHover
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Handles any special actions that the unit needs
	//			to do while it is being hovered over.
	///////////////////////////////////////////////////////////////
	void Draw(void);

	///////////////////////////////////////////////////////////////
	// Function: OnCollision
	//
	// In: void
	//
	// Out: IObject pointer to the object that this unit is 
	//		colliding with and a Vector for the amount of overlap.
	//
	// Purpose: Handles the collision response of the thing walking 
	//			over a certain tile.
	///////////////////////////////////////////////////////////////
	void OnCollision(IObject* Object, D3DXVECTOR2 Overlap);

	///////////////////////////////////////////////////////////////
	// Function: InitMap
	//
	// In: A char pointer for thename of the map file.
	//
	// Out: A CTileStage pointer to the tile stage that was just created.
	//
	// Purpose: Creates the tile map, any static objects on the map
	//			and the pathnodes for the unit ai to use.
	///////////////////////////////////////////////////////////////
	CTileStage* InitMap(const char* szMapFile,  CObjectManager* pManager, int playerNum = 1);

	
	///////////////////////////////////////////////////////////////
	// Function: ShutdownMap
	//
	// In: void
	//
	// Out: void
	//
	// Purpose: Destroys the instance of the TileStage.
	///////////////////////////////////////////////////////////////
	static void DeleteInstance();

	///////////////////////////////////////////////////////////////
	// Function: GetZDepth
	//
	// In: A char pointer for thename of the map file.
	//
	// Out: A CTileStage pointer to the tile stage that was just created.
	//
	// Purpose: Creates the tile map, any static objects on the map
	//			and the pathnodes for the unit ai to use.
	///////////////////////////////////////////////////////////////
	float GetZDepth() const;

	///////////////////////////////////////////////////////////////
	// Function: GetDrawRectangle
	//
	// In: void
	//
	// Out:The rectangle of the map
	//
	// Purpose: Gets the draw rectangle of the tilemap.
	///////////////////////////////////////////////////////////////
	RECT GetDrawRectangle(void) const;

	////////////////////Accessors and Mutators/////////////////////
	char* GetName(void)	const			{ return m_szName; }

	CTile** GetTiles(void) const		{ return m_arrTiles; }
	void SetTiles(CTile** arrTiles)		{ m_arrTiles = arrTiles; }

	int GetMapWidth(void) const			{ return m_nMapWidth; }
	void SetMapWidth(int nMapWidth)		{ m_nMapWidth = nMapWidth; }

	int GetMapHeight(void) const		{ return m_nMapHeight; }
	void SetMapHeight(int nMapHeight)	{ m_nMapHeight = nMapHeight; }

	int GetTileWidth(void) const		{ return m_nTileWidth; }
	void SetTileWidth(int nTileWidth)	{ m_nTileWidth = nTileWidth; }

	int GetTileHeight(void) const		{ return m_nTileHeight; }
	void SetTileHeight(int nTileHeight)	{ m_nTileHeight = nTileHeight; }

	std::vector<CStove*>& GetStoves();
	std::vector<CPlate*>& GetPlates();
};
#endif