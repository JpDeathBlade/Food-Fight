//////////////////////////////////////////////////////
// File: "CSpriteManager.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/14/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CSpriteManager.h"
#include "CSprite.h"
#include <fstream>
CSpriteManager* CSpriteManager::m_Instance = NULL;

CSpriteManager::CSpriteManager()
{

}

CSpriteManager::~CSpriteManager()
{
	UnloadAllSprites();
}

CSpriteManager* CSpriteManager::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CSpriteManager;
	return m_Instance;
}

void CSpriteManager::DeleteInstance()
{
	delete m_Instance;
	m_Instance = NULL;
}

bool CSpriteManager::LoadSpriteFromImage(const char* FilePath, D3DCOLOR _color)
{
	// Search if the sprite already exists.
	for(std::map<std::string, CSprite*>::iterator Iterator = m_Sprites.begin(); Iterator != m_Sprites.end(); Iterator++)
		if(Iterator->first == FilePath)
			return true;

	// Load the texture into memory.
	IDirect3DTexture9* Texture = NULL;
	D3DXIMAGE_INFO ImageInfo;
	D3DXCreateTextureFromFileEx(CSGD_Direct3D::GetInstance()->GetDirect3DDevice(), FilePath, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, _color, &ImageInfo, NULL, &Texture);
	if(!Texture)
		return false;

	// Setup frames.
	RECT Rectangle;
	Rectangle.left = 0;
	Rectangle.right = ImageInfo.Width;
	Rectangle.top = 0;
	Rectangle.bottom = ImageInfo.Height;

	std::vector<CAnchorPoint> AnchorPoints;
	AnchorPoints.resize(1);

	POINT Point;
	Point.x = 0;
	Point.y = 0;
	AnchorPoints[0] = CAnchorPoint("", Point);

	std::vector<CFrame> Frames;
	Frames.resize(1);
	Frames[0] = CFrame("", 0, Rectangle, Rectangle, AnchorPoints);

	// Setup animations.
	std::vector<CAnimation> Animations;
	Animations.resize(1);
	Animations[0] = CAnimation("", Frames);

	// Setup sprite.
	m_Sprites[FilePath] = new CSprite(Texture, Animations);
	return true;
}

bool CSpriteManager::LoadSpriteFromSprite(const char* FilePath)
{
	// Search if the sprite already exists.
	for(std::map<std::string, CSprite*>::iterator Iterator = m_Sprites.begin(); Iterator != m_Sprites.end(); Iterator++)
		if(Iterator->first == FilePath)
			return false;

	// Load the texture into memory.
	std::ifstream SpriteFile(FilePath, std::ios_base::in | std::ios_base::binary);
	IDirect3DTexture9* Texture = NULL;
	long long TextureDataSize = 0;
	SpriteFile.read(reinterpret_cast<char*>(&TextureDataSize), sizeof(long long)); // Byte Length of Texture Data
	char* TextureDataBuffer = new char[static_cast<unsigned int>(TextureDataSize)];
	SpriteFile.read(TextureDataBuffer, static_cast<unsigned int>(TextureDataSize)); // Raw Texture Data
	
	D3DXCreateTextureFromFileInMemoryEx(CSGD_Direct3D::GetInstance()->GetDirect3DDevice(), TextureDataBuffer, static_cast<UINT>(TextureDataSize), 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &Texture);
	if(!Texture)
		return false;

	delete[] TextureDataBuffer;

	// General setup.
	int NumAnimations;
	SpriteFile.read(reinterpret_cast<char*>(&NumAnimations), sizeof(int)); // Number of Animations
	std::vector<CAnimation> Animations;
	Animations.resize(NumAnimations);
	for(int i = 0; i < NumAnimations; ++i)
	{
		int StringD3DXVECTOR2;
		SpriteFile.read(reinterpret_cast<char*>(&StringD3DXVECTOR2), sizeof(int)); // Byte Length of String Data
		char* StringData = new char[StringD3DXVECTOR2 + 1];
		SpriteFile.read(StringData, StringD3DXVECTOR2); // String Data
		StringData[StringD3DXVECTOR2] = '\0';

		int NumFrames;
		SpriteFile.read(reinterpret_cast<char*>(&NumFrames), sizeof(int)); // Number of Frames
		std::vector<CFrame> Frames;
		Frames.resize(NumFrames);
		for(int j = 0; j < NumFrames; ++j)
		{
			int StringD3DXVECTOR2;
			SpriteFile.read(reinterpret_cast<char*>(&StringD3DXVECTOR2), sizeof(int)); // Byte Length of String Data
			char* StringData = new char[StringD3DXVECTOR2 + 1];
			SpriteFile.read(StringData, StringD3DXVECTOR2); // String Data
			StringData[StringD3DXVECTOR2] = '\0';

			short Duration;
			SpriteFile.read(reinterpret_cast<char*>(&Duration), sizeof(short)); // Frame Duration

			int Measure;
			RECT DrawRectangle;
			SpriteFile.read(reinterpret_cast<char*>(&DrawRectangle.left), sizeof(int)); // Draw Rectangle Left
			SpriteFile.read(reinterpret_cast<char*>(&DrawRectangle.top), sizeof(int)); // Draw Rectangle Top
			SpriteFile.read(reinterpret_cast<char*>(&Measure), sizeof(int)); // Draw Rectangle Width
			DrawRectangle.right = DrawRectangle.left + Measure;
			SpriteFile.read(reinterpret_cast<char*>(&Measure), sizeof(int)); // Draw Rectangle Height
			DrawRectangle.bottom = DrawRectangle.top + Measure;

			RECT CollisionRectangle;
			SpriteFile.read(reinterpret_cast<char*>(&CollisionRectangle.left), sizeof(int)); // Collision Rectangle Left
			SpriteFile.read(reinterpret_cast<char*>(&CollisionRectangle.top), sizeof(int)); // Collision Rectangle Top
			SpriteFile.read(reinterpret_cast<char*>(&Measure), sizeof(int)); // Collision Rectangle Width
			CollisionRectangle.right = CollisionRectangle.left + Measure;
			SpriteFile.read(reinterpret_cast<char*>(&Measure), sizeof(int)); // Collision Rectangle Height
			CollisionRectangle.bottom = CollisionRectangle.top + Measure;

			int NumAnchorPoints;
			SpriteFile.read(reinterpret_cast<char*>(&NumAnchorPoints), sizeof(int)); // Number of Anchor Points
			std::vector<CAnchorPoint> AnchorPoints;
			AnchorPoints.resize(NumAnchorPoints);
			for(int k = 0; k < NumAnchorPoints; ++k)
			{
				POINT AnchorPoint;

				int StringD3DXVECTOR2;
				SpriteFile.read(reinterpret_cast<char*>(&StringD3DXVECTOR2), sizeof(int)); // Byte Length of String Data
				char* StringData = new char[StringD3DXVECTOR2 + 1];
				SpriteFile.read(StringData, StringD3DXVECTOR2); // String Data
				StringData[StringD3DXVECTOR2] = '\0';

				SpriteFile.read(reinterpret_cast<char*>(&AnchorPoint.x), sizeof(int)); // AnchorPoint X
				SpriteFile.read(reinterpret_cast<char*>(&AnchorPoint.y), sizeof(int)); // AnchorPoint Y

				AnchorPoints[k] = CAnchorPoint(StringData, AnchorPoint);
				delete StringData;
			}
			Frames[j] = CFrame(StringData, Duration, DrawRectangle, CollisionRectangle, AnchorPoints);
			delete StringData;
		}
		Animations[i] = CAnimation(StringData, Frames);
		delete StringData;
	}
	SpriteFile.close();

	m_Sprites[FilePath] = new CSprite(Texture, Animations);
	return false;
}

bool CSpriteManager::UnloadSprite(const char* FilePath)
{
	for(std::map<std::string, CSprite*>::iterator Iterator = m_Sprites.begin(); Iterator != m_Sprites.end(); Iterator++)
		if(Iterator->first == FilePath)
		{
			delete Iterator->second;
			Iterator->second = NULL;
			Iterator = m_Sprites.erase(Iterator);
			return true;
		}
		return false;
}

void CSpriteManager::UnloadAllSprites()
{
	for(std::map<std::string, CSprite*>::iterator Iterator = m_Sprites.begin(); Iterator != m_Sprites.end(); Iterator++)
	{
		delete Iterator->second;
		Iterator->second = NULL;
	}
	m_Sprites.clear();
}

CSprite* CSpriteManager::GetSprite(const char* FilePath)
{
	for(std::map<std::string, CSprite*>::iterator Iterator = m_Sprites.begin(); Iterator != m_Sprites.end(); Iterator++)
		if(Iterator->first == FilePath)

			return Iterator->second;
	return NULL;
}