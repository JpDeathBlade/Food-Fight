#include "CStoveHUD.h"
#include "../../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../../Managers/Bad Chef/Text/CTextManager.h"
#include "../Appliances/CStove.h"
#include "../../Cooking Mini-Game/CPan.h"
#include "CCursor.h"
#include "../Base/CCamera.h"
#include "../../../Managers/Bad Chef/Audio/CAudioManager.h"

// CSmokeDetectorButton //

CSmokeDetectorButton::CSmokeDetectorButton()
{
	
}

CSmokeDetectorButton::~CSmokeDetectorButton()
{

}

void CSmokeDetectorButton::Update(float Delta)
{
	if(CheckIfPanIsBurning(CStoveHUD::GetInstance()->GetTargetStove()->GetTopLeftPan()) || CheckIfPanIsBurning(CStoveHUD::GetInstance()->GetTargetStove()->GetTopRightPan()) || CheckIfPanIsBurning(CStoveHUD::GetInstance()->GetTargetStove()->GetBottomLeftPan()) || CheckIfPanIsBurning(CStoveHUD::GetInstance()->GetTargetStove()->GetBottomRightPan()))
	{
		SetCurrentAnimation("Flashing");
		if(GetAnimationType() != PLAY_LOOPING)
			SetAnimationType(PLAY_LOOPING);

		// TODO: Play sound and play flashing smoke detector animation.
		if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/SmokeDetector.wav", "User Interface")->IsPlaying())
			CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/SmokeDetector.wav", "User Interface")->Play(1);
	}
	else
	{
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/SmokeDetector.wav", "User Interface")->StopPlaying();
		SetAnimationType(PLAY_STATIC);

		/////////////////////////////////////////////////////////////////
		// BUG FIX
		// Reference Bug # BUG-009
		// BUG FIX START
		/////////////////////////////////////////////////////////////////

		SetCurrentFrame("Unlit");

		///////////////////////////////////////////////////////////////////////////
		// BUG FIX END  Reference # BUG-009
		//////////////////////////////////////////////////////////////////////////
	}
	CButton::Update(Delta);
}

void CSmokeDetectorButton::Draw()
{
	IAnimable::DrawAnimation(GetPosition());
}

void CSmokeDetectorButton::OnClick()
{
	CButton::OnClick();
	if(IsEnabled())
		CAMERA->SetPosition(CStoveHUD::GetInstance()->GetTargetStove()->GetPositionX() - 512, CStoveHUD::GetInstance()->GetTargetStove()->GetPositionY() - 384);
}

bool CSmokeDetectorButton::CheckIfPanIsBurning(CPan* Pan)
{
	return Pan->GetStageTimer() < -5 && Pan->GetPanStage() == PAN_DONE;
}

// CStoveHUD //

CStoveHUD* CStoveHUD::m_Instance = NULL;

CStoveHUD::CStoveHUD()
{
	m_Size.x = 224;
	m_Size.y = 128;
	m_FruitSprite = NULL;
	m_MeatSprite = NULL;
	m_EggSprite = NULL;
	m_WheatSprite = NULL;
	m_TargetStove = NULL;
}

CStoveHUD::~CStoveHUD()
{
	if(m_TargetStove)
		m_TargetStove->ReleaseReference();
	m_Instance = NULL;
}

CStoveHUD* CStoveHUD::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CStoveHUD;
	return m_Instance;
}

void CStoveHUD::DeleteInstance()
{
	delete m_Instance;
}

void CStoveHUD::Update(float Delta)
{
	m_SmokeDetectorButton.Update(Delta);
}

void CStoveHUD::Draw()
{
	DrawGUIFrame(this, GetPosition(), m_Size);

	m_SmokeDetectorButton.Draw();

	char Buffer[4];
	m_FruitSprite->Draw(0, 0, GetPosition() + D3DXVECTOR2(32, 48), 0, D3DXVECTOR2(0.5, 0.5));
	_itoa_s(m_TargetStove->GetFruitCount(), Buffer, 4, 10);
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", Buffer, GetPositionX() + 42, GetPositionY() + 28, 0.5, 0.5);
	m_MeatSprite->Draw(0, 0, GetPosition() + D3DXVECTOR2(96, 48), 0, D3DXVECTOR2(0.5, 0.5));
	 _itoa_s(m_TargetStove->GetMeatCount(), Buffer, 4, 10);
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", Buffer, GetPositionX() + 106, GetPositionY() + 28, 0.5, 0.5);
	m_EggSprite->Draw(0, 0, GetPosition() + D3DXVECTOR2(32, 112), 0, D3DXVECTOR2(0.5, 0.5));
	_itoa_s(m_TargetStove->GetEggCount(), Buffer, 4, 10);
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", Buffer, GetPositionX() + 42, GetPositionY() + 92, 0.5, 0.5);
	m_WheatSprite->Draw(0, 0, GetPosition() + D3DXVECTOR2(96, 112), 0, D3DXVECTOR2(0.5, 0.5));
	_itoa_s(m_TargetStove->GetWheatCount(), Buffer, 4, 10);
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", Buffer, GetPositionX() + 106, GetPositionY() + 92, 0.5, 0.5);
}

void CStoveHUD::OnClick()
{
	IClickable::OnClick();
	RECT CursorRectangle = CCursor::GetInstance()->GetCollisionRectangle();
	CursorRectangle.left -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.right -= (LONG)CAMERA->GetPositionX();
	CursorRectangle.top -= (LONG)CAMERA->GetPositionY();
	CursorRectangle.bottom -= (LONG)CAMERA->GetPositionY();

	RECT IntersectionRectangle;
	if(IntersectRect(&IntersectionRectangle, &CursorRectangle, &m_SmokeDetectorButton.GetCollisionRectangle()))
	{
		m_SmokeDetectorButton.OnClick();
	}
}

void CStoveHUD::SetFruitSprite(CSprite* FruitSprite)
{
	m_FruitSprite = FruitSprite;
}

void CStoveHUD::SetMeatSprite(CSprite* MeatSprite)
{
	m_MeatSprite = MeatSprite;
}

void CStoveHUD::SetEggSprite(CSprite* EggSprite)
{
	m_EggSprite = EggSprite;
}

void CStoveHUD::SetWheatSprite(CSprite* WheatSprite)
{
	m_WheatSprite = WheatSprite;
}

CStove* CStoveHUD::GetTargetStove() const
{
	return m_TargetStove;
}

void CStoveHUD::SetTargetStove(CStove* Stove)
{
	if(m_TargetStove)
		m_TargetStove->ReleaseReference();
	m_TargetStove = Stove;
	if(m_TargetStove)
		m_TargetStove->AcquireReference();
}

IObject* CStoveHUD::CreateStoveHUD(CObjectManager* ObjectManager)
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();

	CStoveHUD* StoveHUD = CStoveHUD::GetInstance();
	StoveHUD->SetPosition(1024 - StoveHUD->m_Size.x, 768 - StoveHUD->m_Size.y);

	CSprite* Sprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	if(!Sprite)
		return NULL;
	StoveHUD->SetSprite(Sprite);

	Sprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/SmokeDetector.spr");
	if(!Sprite)
	{
		CStoveHUD::DeleteInstance();
		return NULL;
	}
	StoveHUD->m_SmokeDetectorButton.SetSprite(Sprite);
	StoveHUD->m_SmokeDetectorButton.SetSize(64, 64);
	StoveHUD->m_SmokeDetectorButton.SetPosition(StoveHUD->GetPosition() + D3DXVECTOR2(StoveHUD->m_Size.x, StoveHUD->m_Size.y * 0.5f) - D3DXVECTOR2(40, 0));

	Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceFruit.spr");
	if(!Sprite)
	{
		CStoveHUD::DeleteInstance();
		return NULL;
	}
	StoveHUD->SetFruitSprite(Sprite);

	Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceMeat.spr");
	if(!Sprite)
	{
		CStoveHUD::DeleteInstance();
		return NULL;
	}
	StoveHUD->SetMeatSprite(Sprite);

	Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceEgg.spr");
	if(!Sprite)
	{
		CStoveHUD::DeleteInstance();
		return NULL;
	}
	StoveHUD->SetEggSprite(Sprite);

	Sprite = SpriteManager->GetSprite("Resources/Graphics/Cooking Mini-Game/ResourceWheat.spr");
	if(!Sprite)
	{
		CStoveHUD::DeleteInstance();
		return NULL;
	}
	StoveHUD->SetWheatSprite(Sprite);


	ObjectManager->AddObjectToLayer(4, StoveHUD);
	return StoveHUD;
}

RECT CStoveHUD::GetDrawRectangle() const
{
	RECT DrawRectangle;
	DrawRectangle.left = (LONG)GetPositionX();
	DrawRectangle.right = (LONG)(DrawRectangle.left + m_Size.x);
	DrawRectangle.top = (LONG)GetPositionY();
	DrawRectangle.bottom = (LONG)(DrawRectangle.top + m_Size.y);
	return DrawRectangle;
}

RECT CStoveHUD::GetCollisionRectangle() const
{
	RECT CollisionRectangle;
	CollisionRectangle.left = (LONG)GetPositionX();
	CollisionRectangle.right = (LONG)(CollisionRectangle.left + m_Size.x);
	CollisionRectangle.top = (LONG)GetPositionY();
	CollisionRectangle.bottom = (LONG)(CollisionRectangle.top + m_Size.y);
	return CollisionRectangle;
}

CSmokeDetectorButton* CStoveHUD::GetSmokeDetectorButton()
{
	return &m_SmokeDetectorButton;
}