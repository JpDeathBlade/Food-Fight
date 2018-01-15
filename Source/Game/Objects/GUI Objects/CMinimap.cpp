//////////////////////////////////////////////////////
// File: "CMiniMap.cpp"
// Creator: AB
// Creation Date: 9/18/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "CMiniMap.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../Units/CUnitObject.h"
#include "../Tiles/CTileStage.h"
#include "../Base/CCamera.h"
#include "CCursor.h"
CMiniMap* CMiniMap::m_Instance = NULL;

CMiniMap::CMiniMap()
{
	m_Scale	= 1;
	m_Offset = D3DXVECTOR2(0, 0);

	m_MiniMapSprite = NULL;
}

CMiniMap::~CMiniMap()
{
	m_Instance = NULL;
}

CMiniMap* CMiniMap::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CMiniMap;
	return m_Instance;
}

void CMiniMap::DeleteInstance()
{
	delete m_Instance;
}

void CMiniMap::LoadMinimap(const char* szFilename)
{
	if(CSpriteManager::GetInstance()->LoadSpriteFromImage(szFilename))
		m_MiniMapSprite = CSpriteManager::GetInstance()->GetSprite(szFilename);
}

void CMiniMap::Draw()
{
	D3DXVECTOR2 Size;
	Size.x = 128;
	Size.y = 128;

	DrawGUIFrame(this, GetPosition(), Size);

	if(m_MiniMapSprite)
		m_MiniMapSprite->DrawBitmap(GetPositionX() + 6.4f, GetPositionY() + 6.4f, 0.9f, 0.9f, NULL, 0, 0, 0, D3DCOLOR_XRGB(255, 255, 255));
	DrawIcons();
	RECT CameraViewRectangle = CAMERA->GetViewRect();

	float CameraViewRectangleLeft = (float)CameraViewRectangle.left;
	float CameraViewRectangleRight = (float)CameraViewRectangle.right;
	float CameraViewRectangleTop = (float)CameraViewRectangle.top;
	float CameraViewRectangleBottom = (float)CameraViewRectangle.bottom;

	CTileStage* TileStage = CTileStage::GetInstance();

	CameraViewRectangleLeft = (CameraViewRectangleLeft / (TileStage->GetTileWidth() * TileStage->GetMapWidth()) * (128 - 2 * (6.4f + m_Offset.x))) + GetPositionX() + (6.4f + m_Offset.x);
	CameraViewRectangleRight = (CameraViewRectangleRight / (TileStage->GetTileWidth() * TileStage->GetMapWidth()) * (128 - 2 * (6.4f + m_Offset.x))) + GetPositionX() + (6.4f + m_Offset.x);
	CameraViewRectangleTop = (CameraViewRectangleTop / (TileStage->GetTileHeight() * TileStage->GetMapHeight()) * (128 - 2 * (6.4f + m_Offset.y))) + GetPositionY() + (6.4f + m_Offset.y);
	CameraViewRectangleBottom = (CameraViewRectangleBottom / (TileStage->GetTileHeight() * TileStage->GetMapHeight()) * (128 - 2 * (6.4f + m_Offset.y))) + GetPositionY() + (6.4f + m_Offset.y);

	CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2(CameraViewRectangleLeft, CameraViewRectangleTop), D3DXVECTOR2(CameraViewRectangleRight, CameraViewRectangleTop), 2.0f);
	CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2(CameraViewRectangleRight, CameraViewRectangleTop), D3DXVECTOR2(CameraViewRectangleRight, CameraViewRectangleBottom), 2.0f);
	CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2(CameraViewRectangleRight, CameraViewRectangleBottom), D3DXVECTOR2(CameraViewRectangleLeft, CameraViewRectangleBottom), 2.0f);
	CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2(CameraViewRectangleLeft, CameraViewRectangleBottom), D3DXVECTOR2(CameraViewRectangleLeft, CameraViewRectangleTop), 2.0f);
}

RECT CMiniMap::GetDrawRectangle() const
{
	RECT DrawRectangle;
	DrawRectangle.left = (LONG)GetPositionX();
	DrawRectangle.right = DrawRectangle.left + 128;
	DrawRectangle.top = (LONG)GetPositionY();
	DrawRectangle.bottom = DrawRectangle.top + 128;
	return DrawRectangle;
}

RECT CMiniMap::GetCollisionRectangle() const
{
	RECT CollisionRectangle;
	CollisionRectangle.left = (LONG)GetPositionX();
	CollisionRectangle.right = CollisionRectangle.left + 128;
	CollisionRectangle.top = (LONG)GetPositionY();
	CollisionRectangle.bottom = CollisionRectangle.top + 128;
	return CollisionRectangle;
}

void CMiniMap::DrawIcons()
{
	m_ObjectManager->IterateLayer<CMiniMap>(1, &ObjectIterator, this);
}

bool CMiniMap::ObjectIterator(IObject* Object, CMiniMap* Caller)
{
	CUnitObject* AsUnitObject = dynamic_cast<CUnitObject*>(Object);
	if(AsUnitObject)
	{
		D3DXVECTOR2 Position = AsUnitObject->GetPosition();
		CTileStage* TileStage = CTileStage::GetInstance();
		Position.x /= TileStage->GetTileWidth() * TileStage->GetMapWidth();
		Position.y /= TileStage->GetTileHeight() * TileStage->GetMapHeight();
		Position.x *= 128 - 2 * (6.4f + Caller->m_Offset.x);
		Position.y *= 128 - 2 * (6.4f + Caller->m_Offset.y);
		Position += Caller->GetPosition() + D3DXVECTOR2(6.4f, 6.4f) + Caller->m_Offset;

		D3DCOLOR Color;
		switch(AsUnitObject->GetTeamValue())
		{
		case TEAM_PLAYER:
			Color = D3DCOLOR_XRGB(127, 255, 127);
			break;
		case TEAM_ENEMY:
			Color = D3DCOLOR_XRGB(255, 127, 127);
			break;
		case TEAM_NONE:
			Color = D3DCOLOR_XRGB(63, 63, 63);
			break;
		default:
			Color = D3DCOLOR_XRGB(0, 0, 0);
		}
		CSGD_Direct3D::GetInstance()->DrawLine(Position + D3DXVECTOR2(0, -1), Position + D3DXVECTOR2(0, 1), 2.0f, Color);
		return false;
	}
	return false;
}

void CMiniMap::OnClick()
{
	// Convert from world-space to screen-space.
	D3DXVECTOR2 Position = CCursor::GetInstance()->GetPosition();
	Position.x -= CAMERA->GetPositionX();
	Position.y -= CAMERA->GetPositionY();

	// LERP from screen-space to MiniMap-space.
	Position.x = (Position.x - GetPositionX() - (6.4f + m_Offset.x)) / (128 - 2 * (6.4f + m_Offset.x));
	Position.y = (Position.y - GetPositionY() - (6.4f + m_Offset.y)) / (128 - 2 * (6.4f + m_Offset.y));

	// LERP from MiniMap space to world-space.
	CTileStage* TileStage = CTileStage::GetInstance();
	Position.x *= TileStage->GetTileWidth() * TileStage->GetMapWidth();
	Position.y *= TileStage->GetTileHeight() * TileStage->GetMapHeight();

	// Offset to center view on point.
	Position.x -= CAMERA->GetClientWidth() * 0.5f;
	Position.y -= CAMERA->GetClientHeight() * 0.5f;

	CAMERA->SetPositionX(Position.x);
	CAMERA->SetPositionY(Position.y);
}

void CMiniMap::SetObjectManager(CObjectManager* const ObjectManager)
{
	m_ObjectManager = ObjectManager;
}

void CMiniMap::SetScale(float Scale)
{
	m_Scale = Scale;
}

void CMiniMap::SetOffset(const D3DXVECTOR2 Offset)
{
	m_Offset = Offset;
}

void CMiniMap::SetOffset(float X, float Y)
{
	SetOffsetX(X);
	SetOffsetY(Y);
}

void CMiniMap::SetOffsetX(float X)
{
	m_Offset.x = X;
}

void CMiniMap::SetOffsetY(float Y)
{
	m_Offset.y = Y;
}

IObject* CMiniMap::CreateMiniMap(CObjectManager* ObjectManager)
{
	CSprite* Sprite = CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/Button.spr");
	if(!Sprite)
		return NULL;

	CMiniMap* Minimap = CMiniMap::GetInstance();
	Minimap->SetSprite(Sprite);
	Minimap->SetPosition(0, 640);
	Minimap->SetObjectManager(ObjectManager);

	ObjectManager->AddObjectToLayer(4, Minimap);
	return Minimap;
}