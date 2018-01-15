#ifndef _CSprite_H_
#define _CSprite_H_
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <string>

class CSGD_Direct3D;
class CAnimation;

class CSprite
{
	CSGD_Direct3D* m_Direct3D;
	IDirect3DTexture9* m_Texture;
	std::vector<CAnimation> m_Animations;
public:
	CSprite(); // Default Constructor
	CSprite(IDirect3DTexture9* Texture, std::vector<CAnimation> Animations); // Custom Constructor
	CSprite(const CSprite& Source); // Copy Constructor
	CSprite& operator=(const CSprite& Source); // Assignment Operator
	~CSprite(); // Destructor

	CAnimation* GetAnimation(const unsigned char Index);
	CAnimation* GetAnimation(std::string _string);

	unsigned char GetNumAnimations() const;

	void Draw(D3DXVECTOR2 Position, float Rotation = 0, D3DXVECTOR2 Scale = D3DXVECTOR2(1, 1), D3DCOLOR Color = D3DCOLOR_XRGB(255, 255, 255), RECT* SourceRectangle = NULL);
	void Draw(D3DXMATRIX Transform, D3DCOLOR Color = D3DCOLOR_XRGB(255, 255, 255), RECT* SourceRectangle = NULL);
	void Draw(unsigned char AnimationIndex, unsigned char FrameIndex, D3DXVECTOR2 Position, float Rotation = 0, D3DXVECTOR2 Scale = D3DXVECTOR2(1, 1), D3DCOLOR Color = D3DCOLOR_XRGB(255, 255, 255));
	void Draw(unsigned char AnimationIndex, unsigned char FrameIndex, D3DXMATRIX Transform, D3DCOLOR Color = D3DCOLOR_XRGB(255, 255, 255));

	void DrawBitmap(float _posX, float _posY, float _scaleX, float _scaleY, RECT* _cell,
		float _rotCenterX, float _rotCenterY, float _rotation, D3DCOLOR _color);
};

class CFrame;

class CAnimation
{
	std::string m_String;
	std::vector<CFrame> m_Frames;
public:
	CAnimation(); // Default Constructor
	CAnimation(std::string String, std::vector<CFrame> Frames); // Custom Constructor
	CAnimation(const CAnimation& Source); // Copy Constructor
	CAnimation& operator=(const CAnimation& Source); // Assignment Operator
	~CAnimation(); // Destructor

	std::string GetString() const;
	CFrame* GetFrame(const unsigned char Index);
	CFrame* GetFrame(std::string _string);
	unsigned char GetNumFrames() const;
	int GetDuration() const;
};

class CAnchorPoint;

class CFrame
{
	std::string m_String;
	short m_Duration;
	RECT m_DrawRectangle;
	RECT m_CollisionRectangle;
	std::vector<CAnchorPoint> m_AnchorPoints;
public:
	CFrame(); // Default Constructor
	CFrame(std::string String, const short Duration, const RECT DrawRectangle, const RECT CollisionRectangle, std::vector<CAnchorPoint> AnchorPoints); // Custom Constructor
	CFrame(const CFrame& Source); // Copy Constructor
	CFrame& operator=(const CFrame& Source); // Assignment Operator
	~CFrame(); // Destructor

	std::string GetString() const;
	short GetDuration() const;
	RECT GetDrawRectangle() const;
	RECT GetCollisionRectangle() const;
	CAnchorPoint* GetAnchorPoint(const unsigned char Index);
	CAnchorPoint* GetAnchorPoint(std::string _string);
	unsigned char GetNumAnchorPoints() const;
};

class CAnchorPoint
{
	std::string m_String;
	POINT m_AnchorPoint;
public:
	CAnchorPoint(); // Default Constructor
	CAnchorPoint(std::string String, POINT AnchorPoint); // Custom Constructor
	CAnchorPoint(const CAnchorPoint& Source); // Copy Constructor
	CAnchorPoint& operator=(const CAnchorPoint& Source); // Assignment Operator
	~CAnchorPoint(); // Destructor

	std::string GetString() const;
	POINT GetAnchorPoint() const;
};
#endif