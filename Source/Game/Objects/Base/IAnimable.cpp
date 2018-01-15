//////////////////////////////////////////////////////
// File: "IAnimable.cpp"
// Creator: SA
// Creation Date: 9/??/09
// Last Modified: --
// Last Modified By: --
//////////////////////////////////////////////////////
#include "IAnimable.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"

IAnimable::IAnimable()
{
	m_CurrentAnimation = 0;
	m_CurrentFrame = 0;
	m_Sprite = NULL;
	m_AnimationLifeTime = 0;
	m_AnimationType = PLAY_ONCE;
	m_Direction = DIRECTION_RIGHT;
	m_State = STATE_NONE;
}

IAnimable::IAnimable(const IAnimable& Source)
{
	m_CurrentAnimation = Source.m_CurrentAnimation;
	m_CurrentFrame = Source.m_CurrentFrame;
	m_Sprite = Source.m_Sprite;
	m_AnimationLifeTime = Source.m_AnimationLifeTime;
	m_AnimationType = Source.m_AnimationType;
	m_Direction = Source.m_Direction;
	m_State = Source.m_State;
}

IAnimable& IAnimable::operator=(const IAnimable& Source)
{
	m_CurrentAnimation = Source.m_CurrentAnimation;
	m_CurrentFrame = Source.m_CurrentFrame;
	m_Sprite = Source.m_Sprite;
	m_AnimationLifeTime = Source.m_AnimationLifeTime;
	m_AnimationType = Source.m_AnimationType;
	m_Direction = Source.m_Direction;
	m_State = Source.m_State;

	return *this;
}

IAnimable::~IAnimable()
{

}

void IAnimable::UpdateAnimation(float Delta)
{
	Delta = Delta * 1000;
	CAnimation* currentAnimation = GetCurrentAnimation();
	int currentTime = 0;

	if(m_AnimationType != PLAY_STATIC)
	{
		m_AnimationLifeTime += Delta;
	}

	// Calculate state and Direction
	switch(m_Direction)
	{
	case DIRECTION_RIGHT:
		{
			switch(m_State)
			{
			case STATE_IDLE:
				{
					if(currentAnimation->GetString() != "State.Idle.Right")
						SetCurrentAnimation("State.Idle.Right");
				}
				break;

			case STATE_MOVING:
				{
					if(currentAnimation->GetString() != "State.Moving.Right")
						SetCurrentAnimation("State.Moving.Right");
				}
				break;

			case STATE_ATTACK:
				{
					if(currentAnimation->GetString() != "State.Attack.Right")
						SetCurrentAnimation("State.Attack.Right");
				}
				break;

			case STATE_DEAD:
				{
					if(currentAnimation->GetString() != "State.Dead.Right")
						SetCurrentAnimation("State.Dead.Right");
				}
				break;

			default:
				break;
			}
		}
		break;

	case DIRECTION_LEFT:
		{
			switch(m_State)
			{
			case STATE_IDLE:
				{
					if(currentAnimation->GetString() != "State.Idle.Left")
						SetCurrentAnimation("State.Idle.Left");
				}
				break;

			case STATE_MOVING:
				{
					if(currentAnimation->GetString() != "State.Moving.Left")
						SetCurrentAnimation("State.Moving.Left");
				}
				break;

			case STATE_ATTACK:
				{
					if(currentAnimation->GetString() != "State.Attack.Left")
						SetCurrentAnimation("State.Attack.Left");
				}
				break;

			case STATE_DEAD:
				{
					if(currentAnimation->GetString() != "State.Dead.Left")
						SetCurrentAnimation("State.Dead.Left");
				}
				break;

			default:
				break;
			}
		}
		break;

	default:
		break;
	}

	if(m_AnimationType != PLAY_STATIC)
	{
		if(currentAnimation)
		{
			for(int i = 0; i < currentAnimation->GetNumFrames(); i++)
			{
				if(m_AnimationLifeTime >= currentTime && 
					m_AnimationLifeTime < (currentTime + currentAnimation->GetFrame(i)->GetDuration()))
				{
					m_CurrentFrame = i;
					break;
				}
				else
					currentTime += currentAnimation->GetFrame(i)->GetDuration();
			}

			if(m_AnimationLifeTime > currentAnimation->GetDuration() && m_AnimationType == PLAY_ONCE)
			{	
				m_AnimationType = PLAY_STATIC;
				OnEndAnimate();
			}

			if(m_AnimationLifeTime > currentAnimation->GetDuration() && m_AnimationType == PLAY_LOOPING)
			{
				m_AnimationLifeTime = 0;
			}
		}
	}
}

void IAnimable::DrawAnimation(D3DXVECTOR2 Position, float Rotation, D3DXVECTOR2 Scale, D3DCOLOR Color)
{
	CSprite* Sprite = GetSprite();
	if(!Sprite)
		return;

	if(m_CurrentFrame > Sprite->GetAnimation(m_CurrentAnimation)->GetNumFrames())
	{
		m_CurrentFrame = 0;
	}

	Sprite->Draw(m_CurrentAnimation, m_CurrentFrame, Position, Rotation, Scale, Color);
}

CAnimation* IAnimable::GetCurrentAnimation() const
{
	CSprite* Sprite = GetSprite();
	if(!Sprite)
		return NULL;
	return Sprite->GetAnimation(m_CurrentAnimation);
}

void IAnimable::SetCurrentAnimation(unsigned char Index)
{
	m_CurrentAnimation = 0;
	m_CurrentFrame = 0;

	CSprite* Sprite = GetSprite();
	if(Sprite)
	{
		if(Index > 0 && Index < Sprite->GetNumAnimations())
			m_CurrentAnimation = Index;
	}
}

void IAnimable::SetCurrentAnimation(char* _string)
{
	m_CurrentAnimation = 0;
	m_CurrentFrame = 0;

	CSprite* Sprite = GetSprite();
	if(Sprite)
	{
		for(int i = 0; i < Sprite->GetNumAnimations(); i++)
		{
			if(!stricmp(m_Sprite->GetAnimation(i)->GetString().c_str(), _string))
			{
				m_CurrentAnimation = i;
			}
		}
	}
}

void IAnimable::SetCurrentFrame(unsigned char Index)
{
	m_CurrentFrame = 0;

	if(m_Sprite)
	{
		if(Index > 0 && Index < GetCurrentAnimation()->GetNumFrames())
			m_CurrentFrame = Index;
	}
}

void IAnimable::SetCurrentFrame(char* _string)
{
	m_CurrentFrame = 0;

	if(m_Sprite)
	{
		for(int i = 0; i < GetCurrentAnimation()->GetNumFrames(); i++)
		{
			if(!stricmp(GetCurrentAnimation()->GetFrame(i)->GetString().c_str(), _string))
			{
				m_CurrentFrame = i;
			}
		}
	}
}

CFrame* IAnimable::GetCurrentFrame() const
{
	CSprite* Sprite = GetSprite();
	if(!Sprite)
		return NULL;
	return Sprite->GetAnimation(m_CurrentAnimation)->GetFrame(m_CurrentFrame);
}

CSprite* IAnimable::GetSprite() const
{
	return m_Sprite;
}

void IAnimable::SetSprite(CSprite* Sprite)
{
	m_Sprite = Sprite;
}