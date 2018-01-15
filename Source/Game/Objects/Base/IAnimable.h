#ifndef _IAnimable_H_
#define _IAnimable_H_
#include <d3dx9.h>

#define getA(c) (((c)&0xff000000)>>24)
#define getR(c) (((c)&0x00ff0000)>>16)
#define getG(c) (((c)&0x0000ff00)>>8)
#define getB(c) ((c)&0x000000ff)

class CSprite;
class CAnimation;
class CFrame;

class IAnimable
{
public:
	enum EAnimation			{ PLAY_ONCE, PLAY_LOOPING, PLAY_STATIC, PLAY_MAX };
	enum EUnitDirection		{ DIRECTION_UP, DIRECTION_RIGHT, DIRECTION_DOWN, DIRECTION_LEFT };
	enum EUnitState			{ STATE_IDLE, STATE_MOVING, STATE_ATTACK, STATE_DEAD, STATE_NONE };

private:
	unsigned char m_CurrentAnimation;
	unsigned char m_CurrentFrame;
	CSprite* m_Sprite;
	float m_AnimationLifeTime;
	EAnimation m_AnimationType;
	EUnitDirection m_Direction;
	EUnitState m_State;

public:
	IAnimable(); // Default Constructor
	IAnimable(const IAnimable& Source); // Copy Constructor
	virtual IAnimable& operator=(const IAnimable& Source); // Assignment Operator
	virtual ~IAnimable() = 0; // Destructor

	void UpdateAnimation(float Delta);
	void DrawAnimation(D3DXVECTOR2 Position, float Rotation = 0, D3DXVECTOR2 Scale = D3DXVECTOR2(1, 1), D3DCOLOR Color = D3DCOLOR_XRGB(255, 255, 255));

	CAnimation* GetCurrentAnimation() const;

	void SetCurrentAnimation(unsigned char Index);
	void SetCurrentAnimation(char* _string);

	void SetCurrentFrame(unsigned char Index);
	void SetCurrentFrame(char* _string);

	CFrame* GetCurrentFrame() const;

	CSprite* GetSprite() const;
	virtual void SetSprite(CSprite* Sprite);

	void SetAnimationType(EAnimation _type) { m_AnimationType = _type; }
	void SetState(EUnitState _state) { m_State = _state; }
	void SetDirection(EUnitDirection _direction) { m_Direction = _direction; } 

	virtual void OnEndAnimate() {}

	EAnimation GetAnimationType()	{ return m_AnimationType; }
	EUnitDirection GetDirection()	{ return m_Direction;	  }
	EUnitState GetState()			{ return m_State;		  }
};
#endif