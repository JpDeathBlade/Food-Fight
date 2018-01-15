//////////////////////////////////////////////////////
// File: "CTileStage.cpp"
// Creator: AB
// Creation Date: 9/15/09
// Last Modified: 9/19/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CTileStage.h"
#include "CTile.h"
#include "../GUI Objects/CMiniMap.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../Path Nodes/CPathNode.h"
#include "../../../Managers/SGD/CSGD_Direct3D.h"
#include "../Base/CCamera.h"
#include "../../../Managers/Bad Chef/CObjectFactory.h"
#include "../Appliances/CStove.h"
#include "../Appliances/CPlate.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include <stdio.h>

CTileStage* CTileStage::m_pInstance = NULL;

CTileStage::CTileStage(void)
{
	m_szName		= NULL;
	m_nMapWidth		= -1;
	m_nMapHeight	= -1;
	m_nTileWidth	= -1;
	m_nTileHeight	= -1;
	m_nTilesetWidth = 2;
	m_texTileSet	= NULL;
	SetCollideable(false);
}

CTileStage::~CTileStage(void)
{
	if(m_szName)
	{
		delete m_szName;
		m_szName = NULL;
	}

	if(m_arrTiles)
	{
		for(int i = 0; i < m_nMapWidth; ++i)
		{
			delete[] m_arrTiles[i];
			m_arrTiles[i] = NULL;
		}

		delete[] m_arrTiles;
		m_arrTiles = NULL;
	}

	if(m_texTileSet)
	{
		m_texTileSet->Release();
		m_texTileSet = NULL;
	}

	/*for(unsigned int i = 0; i < m_Stoves.size(); ++i)
		m_Stoves[i]->ReleaseReference();*/

	m_Stoves.clear();

	/*for(unsigned int i = 0; i < m_Plates.size(); ++i)
		m_Plates[i]->ReleaseReference();*/

	m_Plates.clear();

	//m_pInstance = NULL;
}

CTileStage* CTileStage::GetInstance(void)
{
	if(!m_pInstance)
	{
		m_pInstance = new CTileStage;
		m_pInstance->AcquireReference();
	}

	return m_pInstance;
}

RECT CTileStage::TilesetCellAlgorithm(int nID)
{
	RECT rCell;

	rCell.left = m_nTileWidth * (nID % m_nTilesetWidth);
	rCell.top = m_nTileHeight * (nID / m_nTilesetWidth);
	rCell.right = rCell.left + m_nTileWidth;
	rCell.bottom = rCell.top + m_nTileHeight;

	return rCell;
}

RECT CTileStage::CellAlgorithm(int nID)
{
	RECT rCell;

	rCell.left = m_nTileWidth * (nID % m_nMapWidth);
	rCell.top = m_nTileHeight * (nID / m_nMapWidth);
	rCell.right = rCell.left + m_nTileWidth;
	rCell.bottom = rCell.top + m_nTileHeight;

	return rCell;
}

void CTileStage::Update(float Delta)
{
   
}

void CTileStage::Draw(void)
{
	D3DXVECTOR3 vPos;
	vPos.z = 0.0f;
	RECT rSource;

	int yStart, xStart, yMax, xMax;

	RECT rCamera = CAMERA->GetViewRect();

	//rCamera.bottom -= 100;
	//rCamera.right  -= 100;

	yStart = rCamera.top / m_nTileHeight;
	xStart = rCamera.left / m_nTileWidth;
	yMax = rCamera.bottom / m_nTileHeight;
	xMax = rCamera.right / m_nTileWidth;

	for(int y = yStart; y < yMax+1; ++y)
	{
		for(int x = xStart; x < xMax+1; ++x)
		{
			if(y < m_nMapHeight && x < m_nMapWidth)
			{
				int nTempID = m_arrTiles[x][y].GetTileID();
				if(nTempID != -1)
				{
					vPos.x = (float)(x * m_nTileWidth);
					vPos.y = (float)(y * m_nTileHeight);

					rSource = TilesetCellAlgorithm(nTempID);
					CSGD_Direct3D::GetInstance()->GetSprite()->Draw(m_texTileSet, &rSource,  NULL, &vPos, 0xffffffff);
				}
			}
		}
	}
}

void CTileStage::OnCollision(IObject* Object, D3DXVECTOR2 Overlap)
{

}

float CTileStage::GetZDepth() const
{
	return 0;
}

CTileStage* CTileStage::InitMap(const char* szMapFile, CObjectManager* pManager, int playerNum)
{
	CMiniMap* pNewMinimap = NULL;
	pNewMinimap = static_cast<CMiniMap*>(CMiniMap::CreateMiniMap(pManager));

	if(m_pInstance)
	{
		FILE* pFile = NULL;

		fopen_s(&pFile, szMapFile, "rb");

		if(pFile)
		{
			int cameraPosX = 0;
			int cameraPosY = 0;

			int nNameLength;
			
			fread(&nNameLength, sizeof(int), 1, pFile);

			m_szName = new char[nNameLength + 1];

			fread(m_szName, sizeof(char), nNameLength, pFile);

			m_szName[nNameLength] = '\0';
			
			fread(&m_nMapWidth, sizeof(int), 1, pFile);
			fread(&m_nMapHeight, sizeof(int), 1, pFile);

			fread(&nNameLength, sizeof(int), 1, pFile);

			char* pTemp = new char[nNameLength + 1];

			fread(pTemp, sizeof(char), nNameLength, pFile);

			pTemp[nNameLength] = '\0';


			char szFileName[128];

			sprintf_s(szFileName, 128, "Resources/Graphics/User Interface/%s", pTemp);

			if(pTemp)
			{
				pNewMinimap->LoadMinimap(szFileName);

				delete pTemp;
				pTemp = NULL;
			}
			else
				pNewMinimap->LoadMinimap("Resources/Graphics/User Interface/ff_ErrorMinimap.png");

			int nOffset = 0;

			fread(&nOffset, sizeof(int), 1, pFile);
			pNewMinimap->SetOffsetX((float)nOffset);

			fread(&nOffset, sizeof(int), 1, pFile);
			pNewMinimap->SetOffsetY((float)nOffset);

			float fScale = 1.0f;

			fread(&fScale, sizeof(float), 1, pFile);
			pNewMinimap->SetScale(fScale);

			pNewMinimap->SetPositionX(0.0f);
			pNewMinimap->SetPositionY(CAMERA->GetClientHeight() - 128);

			pManager->AddObjectToLayer(4, pNewMinimap);
			
			fread(&nNameLength, sizeof(int), 1, pFile);
			pTemp = new char[nNameLength + 1];
			fread(pTemp, sizeof(char), nNameLength, pFile);

			pTemp[nNameLength] = '\0';

			sprintf_s(szFileName, 128, "Resources/Graphics/Tilesets/%s", pTemp);

			D3DXCreateTextureFromFile(CSGD_Direct3D::GetInstance()->GetDirect3DDevice(), szFileName, &m_texTileSet);

			if(pTemp)
			{
				delete pTemp;
				pTemp = NULL;
			}

			fread(&m_nTileWidth, sizeof(int), 1, pFile);
			fread(&m_nTileHeight, sizeof(int), 1, pFile);

			m_arrTiles = new CTile*[m_nMapWidth];

			for(int y = 0; y < m_nMapWidth; ++y)
			{
				m_arrTiles[y] = new CTile[m_nMapHeight];

				for(int x = 0; x < m_nMapHeight; ++x)
				{
					int nTileID			= -1;
					int nCollisionID	= -1;

					fread(&nTileID, sizeof(int), 1, pFile);
					fread(&nCollisionID, sizeof(int), 1, pFile);

					m_arrTiles[y][x].SetTileID(nTileID);
					m_arrTiles[y][x].SetCollisionID(nCollisionID);
				}
			}

			pManager->AddObjectToLayer(0, m_pInstance);

			int nNumObjects = 0;

			fread(&nNumObjects, sizeof(int), 1, pFile);

			int		nIDLength, nPosX, nPosY, nNumProperties, nPropertyIDLength, nPropertyValueLength;
			char*	szObjectID, *szPropertyID, *szPropertyValue;
			nIDLength = nPosX = nPosY = nNumProperties = nPropertyIDLength = nPropertyValueLength = 0;
			szObjectID = szPropertyID = szPropertyValue = NULL;
			CGameObject* pObjectToAdd = NULL;

			for(int i = 0; i < nNumObjects; ++i)
			{
				fread(&nIDLength, sizeof(int), 1, pFile);
				szObjectID = new char[nIDLength + 1];
				fread(szObjectID, sizeof(char), nIDLength, pFile);
				szObjectID[nIDLength] = '\0';

				/////////////////check ids/////////////////

				fread(&nPosX, sizeof(int), 1, pFile);
				fread(&nPosY, sizeof(int), 1, pFile);
				
				if(!strcmp(szObjectID, "stove"))
				{
					pObjectToAdd = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("Stove", pManager));
					m_Stoves.push_back(static_cast<CStove*>(pObjectToAdd));
				}
				else if(!strcmp(szObjectID, "plate"))
				{
					pObjectToAdd = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("Plate", pManager));
					m_Plates.push_back(static_cast<CPlate*>(pObjectToAdd));
				}
				else if(!strcmp(szObjectID, "coffeeMaker"))
					pObjectToAdd = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("CoffeeMaker", pManager));
				else if(!strcmp(szObjectID, "toaster"))
					pObjectToAdd = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("Toaster", pManager));
				else if(!strcmp(szObjectID, "toasterOven"))
					pObjectToAdd = static_cast<CGameObject*>(CObjectFactory::GetInstance()->Construct("toasterOven", pManager));

				if(pObjectToAdd)
					pObjectToAdd->SetPosition((float)nPosX, (float)nPosY);

				fread(&nNumProperties, sizeof(int), 1, pFile);

				for(int j = 0; j < nNumProperties; ++j)
				{
					fread(&nPropertyIDLength, sizeof(int), 1, pFile);
					szPropertyID = new char[nPropertyIDLength + 1];
					fread(szPropertyID, sizeof(char), nPropertyIDLength, pFile);
					szPropertyID[nPropertyIDLength] = '\0';

					////////////check property///////////////
					
					
					fread(&nPropertyValueLength, sizeof(int), 1, pFile);
					szPropertyValue = new char[nPropertyValueLength + 1];
					fread(szPropertyValue, sizeof(char), nPropertyValueLength, pFile);
					szPropertyValue[nPropertyValueLength] = '\0';

					/////////////set value/////////////////////////
					
					if(pObjectToAdd)
					{
					
						if(!strcmp(szPropertyID, "facingValue"))
						{
							
							CStove* pTempStove = dynamic_cast<CStove*>(pObjectToAdd);

							if(pTempStove)
							{
								if(!strcmp(szPropertyValue, "right"))
									pTempStove->InitializeCookingMinigame(true, pManager);
								else if(!strcmp(szPropertyValue, "left"))
									pTempStove->InitializeCookingMinigame(false, pManager);
							}
							
						}
						else if(!strcmp(szPropertyID, "dropRectX"))
						{
							CStove* pTempStove = dynamic_cast<CStove*>(pObjectToAdd);

							if(pTempStove)
							{
								pTempStove->SetSpawnX(atoi(szPropertyValue));
							}
						}
						else if(!strcmp(szPropertyID, "dropRectY"))
						{
							CStove* pTempStove = dynamic_cast<CStove*>(pObjectToAdd);

							if(pTempStove)
							{
								pTempStove->SetSpawnY(atoi(szPropertyValue));
							}
						}
						else if(!strcmp(szPropertyID, "player"))
						{
							CStove* pTempStove = dynamic_cast<CStove*>(pObjectToAdd);

							if(pTempStove)
							{
								if(atoi(szPropertyValue) == playerNum)
								{
									pTempStove->SetTeamValue(TEAM_PLAYER);
									pTempStove->SetNWOwner(true);
									cameraPosX = (int)pTempStove->GetPositionX();
									cameraPosY = (int)pTempStove->GetPositionY();
								}
								else
								{
									pTempStove->SetTeamValue(TEAM_ENEMY);
									pTempStove->SetNWOwner(false);
								}
							}
						}
						else if(dynamic_cast<CPlate*>(pObjectToAdd))
						{
							CPlate* pTempPlate = dynamic_cast<CPlate*>(pObjectToAdd);
							
							if(!strcmp(szPropertyID, "egg"))
								pTempPlate->AddEgg(atoi(szPropertyValue));
							else if(!strcmp(szPropertyID, "meat"))
								pTempPlate->AddMeat(atoi(szPropertyValue));
							else if(!strcmp(szPropertyID, "wheat"))
								pTempPlate->AddWheat(atoi(szPropertyValue));
							else if(!strcmp(szPropertyID, "fruit"))
								pTempPlate->AddFruit(atoi(szPropertyValue));
						}
					}

					if(szPropertyID)
					{
						delete szPropertyID;
						szPropertyID = NULL;
					}

					if(szPropertyValue)
					{
						delete szPropertyValue;
						szPropertyValue = NULL;
					}
				}

				if(szObjectID)
				{
					delete szObjectID;
					szObjectID = NULL;
				}
			}

			int nNumNodes = 0;

			fread(&nNumNodes, sizeof(int), 1, pFile);

			int nNodeID, nNodeTypeLength, nNumLinks, nLinkToID;
			nNodeID = nNodeTypeLength = nPosX = nPosY = nNumLinks = nLinkToID = 0;
			char* szNodeType = NULL;
			CPathNode* pTempNode;

			for(int index = 0; index < nNumNodes; ++index)
			{
				pTempNode = new CPathNode();

				fread(&nNodeID, sizeof(int), 1, pFile);
				
				fread(&nNodeTypeLength, sizeof(int), 1, pFile);
				szNodeType = new char[nNodeTypeLength + 1];
				fread(szNodeType, sizeof(char), nNodeTypeLength, pFile);
				szNodeType[nNodeTypeLength] = '\0';

				fread(&nPosX, sizeof(int), 1, pFile);
				fread(&nPosY, sizeof(int), 1, pFile);

				pTempNode->SetNodeID(nNodeID);
				pTempNode->SetType(PATH_NORMAL);
				pTempNode->SetPosition((float)nPosX, (float)nPosY);
				
				fread(&nNumLinks, sizeof(int), 1, pFile);

				for(int j = 0; j < nNumLinks; ++j)
				{
					fread(&nLinkToID, sizeof(int), 1, pFile);
					pTempNode->AddLinkToSetup(nLinkToID);
				}

				CPathNode::AddNode(pTempNode);

				if(szNodeType)
				{
					delete szNodeType;
					szNodeType = NULL;
				}
			}

			CPathNode::CompilePaths();

			int hudSize = 128;
			CAMERA->InitCamera(1024.0f, 768.0f, (float)(m_nMapWidth * m_nTileWidth), (float)(m_nMapHeight * m_nTileHeight + hudSize), (float)(cameraPosX - 512) , (float)(cameraPosY - 384), 5.0f);
			CAMERA->SetCameraSpeed(1000.0f);

			fclose(pFile);
		}
		else
		{
			pNewMinimap->LoadMinimap("Resources/Graphics/User Interface/ff_ErrorMinimap.png");
			pNewMinimap->SetOffsetX(0);
			pNewMinimap->SetOffsetY(0);
			pNewMinimap->SetScale(1.0f);
		}
	}

	return m_pInstance;
}

RECT CTileStage::GetDrawRectangle() const
{
	RECT rDraw;

	rDraw.left = 0;
	rDraw.top = 0;
	rDraw.bottom = m_nTileHeight * m_nMapHeight;
	rDraw.right = m_nTileWidth * m_nMapWidth;

	return rDraw;
}

void CTileStage::DeleteInstance()
{
	if(m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

std::vector<CStove*>& CTileStage::GetStoves()
{
	return m_Stoves;
}

std::vector<CPlate*>& CTileStage::GetPlates()
{
	return m_Plates;
}