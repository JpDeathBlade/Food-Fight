//////////////////////////////////////////////////////
// File: "CSprite.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: 9/14/09
// Last Modified By: AB
//////////////////////////////////////////////////////
#include "CSprite.h"
#include "..\..\SGD\CSGD_Direct3D.h"

CSprite::CSprite()
{
	m_Direct3D = CSGD_Direct3D::GetInstance();
	m_Texture = NULL;
}

CSprite::CSprite(IDirect3DTexture9* Texture, std::vector<CAnimation> Animations)
{
	m_Direct3D = CSGD_Direct3D::GetInstance();
	m_Texture = Texture;
	m_Animations = Animations;
}

CSprite::CSprite(const CSprite& Source)
{
	m_Direct3D = Source.m_Direct3D;
	m_Texture = Source.m_Texture;
	m_Animations = Source.m_Animations;
}

CSprite& CSprite::operator=(const CSprite& Source)
{
	m_Direct3D = Source.m_Direct3D;
	m_Texture = Source.m_Texture;
	m_Animations = Source.m_Animations;
	return *this;
}

CSprite::~CSprite()
{
	if(m_Texture)
		m_Texture->Release();
	m_Texture = NULL;
}

CAnimation* CSprite::GetAnimation(const unsigned char Index)
{
	return &m_Animations[Index];
}

CAnimation* CSprite::GetAnimation(std::string _string)
{
	for(unsigned int i = 0; i < m_Animations.size(); i++)
	{
		if(m_Animations[i].GetString() == _string)
		{
			return &m_Animations[i];
		}
	}

	return &m_Animations[0];
}

unsigned char CSprite::GetNumAnimations() const
{
	return m_Animations.size();
}

void CSprite::Draw(D3DXVECTOR2 Position, float Rotation, D3DXVECTOR2 Scale, D3DCOLOR Color, RECT* SourceRectangle)
{
	D3DXMATRIX PreviousTransform;
	CSGD_Direct3D::GetInstance()->GetSprite()->GetTransform(&PreviousTransform);

	D3DXMATRIX FinalTransform = PreviousTransform;
	D3DXMATRIX Transformation;
	D3DXMatrixScaling(&Transformation, Scale.x, Scale.y, 1);
	FinalTransform *= Transformation;
	D3DXMatrixRotationZ(&Transformation, Rotation);
	FinalTransform *= Transformation;
	D3DXMatrixTranslation(&Transformation, Position.x, Position.y, 0);
	FinalTransform *= Transformation;
	CSGD_Direct3D::GetInstance()->GetSprite()->SetTransform(&FinalTransform);

	m_Direct3D->GetSprite()->Draw(m_Texture, SourceRectangle, NULL, NULL, Color);

	CSGD_Direct3D::GetInstance()->GetSprite()->SetTransform(&PreviousTransform);
}

void CSprite::Draw(unsigned char AnimationIndex, unsigned char FrameIndex, D3DXVECTOR2 Position, float Rotation, D3DXVECTOR2 Scale, D3DCOLOR Color)
{
	D3DXMATRIX PreviousTransform;
	CSGD_Direct3D::GetInstance()->GetSprite()->GetTransform(&PreviousTransform);

	D3DXMATRIX FinalTransform = PreviousTransform;
	D3DXMATRIX Transformation;
	D3DXMatrixScaling(&Transformation, Scale.x, Scale.y, 1);
	FinalTransform *= Transformation;
	D3DXMatrixRotationZ(&Transformation, Rotation);
	FinalTransform *= Transformation;
	CFrame* Frame = m_Animations[AnimationIndex].GetFrame(FrameIndex);
	RECT DrawRectangle = Frame->GetDrawRectangle();
	POINT AnchorPoint = Frame->GetAnchorPoint(0)->GetAnchorPoint();
	D3DXVECTOR2 Offset = D3DXVECTOR2((float)(DrawRectangle.left - AnchorPoint.x), (float)(DrawRectangle.top - AnchorPoint.y));
	D3DXMatrixTranslation(&Transformation, Position.x + Offset.x, Position.y + Offset.y, 0);
	FinalTransform *= Transformation;
	CSGD_Direct3D::GetInstance()->GetSprite()->SetTransform(&FinalTransform);

	m_Direct3D->GetSprite()->Draw(m_Texture, &DrawRectangle, NULL, NULL, Color);

	CSGD_Direct3D::GetInstance()->GetSprite()->SetTransform(&PreviousTransform);
}

void CSprite::DrawBitmap(float _posX, float _posY, float _scaleX, float _scaleY, RECT* _cell, 
					   float _rotCenterX, float _rotCenterY, float _rotation, D3DCOLOR _color)
{
	D3DXMATRIX save;
	m_Direct3D->GetSprite()->GetTransform(&save);

	D3DXMATRIX scale;
	D3DXMATRIX rotation;
	D3DXMATRIX translate;
	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);

	//ROTATE
	D3DXMatrixTranslation(&translate, -_rotCenterX, -_rotCenterY, 0.0f);
	world *= translate;
	D3DXMatrixRotationZ(&rotation, _rotation);
	world *= rotation;
	D3DXMatrixTranslation(&translate, _rotCenterX, _rotCenterY, 0.0f);
	world *= translate;

	//SCALE
	D3DXMatrixScaling(&scale, _scaleX, _scaleY, 1.0f);
	world *= scale;

	//TRANSLATE
	D3DXMatrixTranslation(&translate, _posX, _posY, 0.0f);
	world *= translate;

	m_Direct3D->GetSprite()->SetTransform(&world);

	//Draw
	m_Direct3D->GetSprite()->Draw(m_Texture, _cell, NULL, NULL, _color);

	m_Direct3D->GetSprite()->SetTransform(&save);
}

CAnimation::CAnimation()
{
	m_String.clear();
}

CAnimation::CAnimation(std::string String, std::vector<CFrame> Frames)
{
	m_String = String;
	m_Frames = Frames;
}

CAnimation::CAnimation(const CAnimation& Source)
{
	m_String = Source.m_String;
	m_Frames = Source.m_Frames;
}

CAnimation& CAnimation::operator=(const CAnimation& Source)
{
	m_String = Source.m_String;
	m_Frames = Source.m_Frames;
	return *this;
}

CAnimation::~CAnimation()
{

}

std::string CAnimation::GetString() const
{
	return m_String;
}

CFrame* CAnimation::GetFrame(const unsigned char Index)
{	
	if(Index >= m_Frames.size())
		return &m_Frames[0];

	return &m_Frames[Index];
}

CFrame* CAnimation::GetFrame(std::string _string)
{
	for(unsigned int i = 0; i < m_Frames.size(); i++)
	{
		if(m_Frames[i].GetString() == _string)
		{
			return &m_Frames[i];
		}
	}

	return &m_Frames[0];
}

unsigned char CAnimation::GetNumFrames() const
{
	return m_Frames.size();
}

int CAnimation::GetDuration() const
{
	int Duration = 0;
	for(unsigned char Index = 0; Index < m_Frames.size(); ++Index)
		Duration += m_Frames[Index].GetDuration();
	return Duration;
}

CFrame::CFrame()
{
	m_String.clear();
	m_Duration = 0;
	m_DrawRectangle.top = 0;					   // A
	m_DrawRectangle.left = 0;					 // W
	m_DrawRectangle.right = 0;				   // E
	m_DrawRectangle.bottom = 0;				 // S
	m_CollisionRectangle.top = 0;		   // O
	m_CollisionRectangle.left = 0;		 // M
	m_CollisionRectangle.right = 0;	   // E
	m_CollisionRectangle.bottom = 0; //	!
}

CFrame::CFrame(std::string String, const short Duration, const RECT DrawRectangle, const RECT CollisionRectangle, const std::vector<CAnchorPoint> AnchorPoints)
{
	m_String = String;
	m_Duration = Duration;
	m_DrawRectangle = DrawRectangle;
	m_CollisionRectangle = CollisionRectangle;
	m_AnchorPoints = AnchorPoints;
}

CFrame::CFrame(const CFrame& Source)
{
	m_String = Source.m_String;
	m_Duration = Source.m_Duration;
	m_DrawRectangle = Source.m_DrawRectangle;
	m_CollisionRectangle = Source.m_CollisionRectangle;
	m_AnchorPoints = Source.m_AnchorPoints;
}

CFrame& CFrame::operator=(const CFrame& Source)
{
	m_String = Source.m_String;
	m_Duration = Source.m_Duration;
	m_DrawRectangle = Source.m_DrawRectangle;
	m_CollisionRectangle = Source.m_CollisionRectangle;
	m_AnchorPoints = Source.m_AnchorPoints;
	return *this;
}

CFrame::~CFrame()
{

}

std::string CFrame::GetString() const
{
	return m_String;
}

short CFrame::GetDuration() const
{
	return m_Duration;
}

RECT CFrame::GetDrawRectangle() const
{
	return m_DrawRectangle;
}

RECT CFrame::GetCollisionRectangle() const
{
	return m_CollisionRectangle;
}

CAnchorPoint* CFrame::GetAnchorPoint(const unsigned char Index)
{
	return &m_AnchorPoints[Index];
}

CAnchorPoint* CFrame::GetAnchorPoint(std::string _string)
{
	for(unsigned int i = 0; i < m_AnchorPoints.size(); i++)
	{
		if(m_AnchorPoints[i].GetString() == _string)
		{
			return &m_AnchorPoints[i];
		}
	}

	return NULL;
}

unsigned char CFrame::GetNumAnchorPoints() const
{
	return m_AnchorPoints.size();
}

CAnchorPoint::CAnchorPoint()
{
	m_AnchorPoint.x = 0;
	m_AnchorPoint.y = 0;
}

CAnchorPoint::CAnchorPoint(std::string String, POINT AnchorPoint)
{
	m_String = String;
	m_AnchorPoint = AnchorPoint;
}

CAnchorPoint::CAnchorPoint(const CAnchorPoint& Source)
{
	m_String = Source.m_String;
	m_AnchorPoint = Source.m_AnchorPoint;
}

CAnchorPoint& CAnchorPoint::operator=(const CAnchorPoint& Source)
{
	m_String = Source.m_String;
	m_AnchorPoint = Source.m_AnchorPoint;
	return *this;
}

CAnchorPoint::~CAnchorPoint()
{

}

std::string CAnchorPoint::GetString() const
{
	return m_String;
}

POINT CAnchorPoint::GetAnchorPoint() const
{
	return m_AnchorPoint;
}