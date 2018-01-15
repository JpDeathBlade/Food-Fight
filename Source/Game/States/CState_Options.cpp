#include "CState_Options.h"
#include "../../Managers/Bad Chef/Sprites/CSpriteManager.h"
#include "../../Managers/Bad Chef/Sprites/CSprite.h"
#include "../../Managers/Bad Chef/Text/CTextManager.h"
#include "../../Managers/Bad Chef/String Table/CStringTable.h"
#include "../../Managers/SGD/CSGD_DirectInput.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include "../../Managers/Bad Chef/XBOX/CXBoxManager.h"
#include "../../CGame.h"
#include "../../Managers/SGD/CSGD_Direct3D.h"
#include "../Objects/GUI Objects/CButton.h"
#include "CState_KeyBinding.h"
#include "../../Managers/Bad Chef/Audio/CAudioManager.h"

CState_Options::CState_Options()
{
	CSpriteManager* SpriteManager = CSpriteManager::GetInstance();
	SpriteManager->LoadSpriteFromSprite("Resources/Graphics/User Interface/Button.spr");

	m_ObjectManager.Initialize(1);
	m_ObjectManager.SetLayerCoordinateSpace(0, CObjectLayer::ECoordinateSpace::CS_Screen);

	CSprite* ButtonSprite = SpriteManager->GetSprite("Resources/Graphics/User Interface/Button.spr");
	SetSprite(ButtonSprite);

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-008
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	NameButtonOnClick.SetOptionsState(this);
	MusicVolumeButtonOnClick.SetOptionsState(this);
	EffectsVolumeButtonOnClick.SetOptionsState(this);
	VoiceVolumeButtonOnClick.SetOptionsState(this);
	UserInterfaceVolumeButtonOnClick.SetOptionsState(this);
	FullscreenButtonOnClick.SetOptionsState(this);
	LanguageButtonOnClick.SetOptionsState(this);

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-008
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	KeyBindingsButtonOnClick.SetOptionsState(this);

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-008
	//////////////////////////////////////////////////////////////////////////

	ApplyButtonOnClick.SetOptionsState(this);
	BackButtonOnClick.SetOptionsState(this);

	NameButtonOnClick.SetIndex(0);
	MusicVolumeButtonOnClick.SetIndex(1);
	EffectsVolumeButtonOnClick.SetIndex(2);
	VoiceVolumeButtonOnClick.SetIndex(3);
	UserInterfaceVolumeButtonOnClick.SetIndex(4);
	FullscreenButtonOnClick.SetIndex(5);
	LanguageButtonOnClick.SetIndex(6);

	/////////////////////////////////////////////////////////////////
	// BUG FIX
	// Reference Bug # BB-008
	// BUG FIX START
	/////////////////////////////////////////////////////////////////

	KeyBindingsButtonOnClick.SetIndex(7);

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-008
	//////////////////////////////////////////////////////////////////////////

	ApplyButtonOnClick.SetIndex(8);
	BackButtonOnClick.SetIndex(9);

	///////////////////////////////////////////////////////////////////////////
	// BUG FIX END  Reference # BB-002
	//////////////////////////////////////////////////////////////////////////

	for(unsigned char i = 0; i < 10; ++i)
	{
		m_Buttons[i] = new CButton();
		m_Buttons[i]->SetSprite(ButtonSprite);
		m_Buttons[i]->SetPosition(110.0f, 160.0f);
		m_Buttons[i]->SetFontFileName("Resources/Data/Font.spr");
		m_Buttons[i]->SetFontScale(0.75f, 0.75f);
		m_Buttons[i]->SetStringOffset(10, 10);
	}

	m_Buttons[0]->SetString(STRING->FindString("Name").c_str());
	m_Buttons[0]->SetOnRelease(&NameButtonOnClick);

	m_Buttons[1]->SetString(STRING->FindString("Music Volume").c_str());
	m_Buttons[1]->SetOnRelease(&MusicVolumeButtonOnClick);

	m_Buttons[2]->SetString(STRING->FindString("Effect Volume").c_str());
	m_Buttons[2]->SetOnRelease(&EffectsVolumeButtonOnClick);

	m_Buttons[3]->SetString(STRING->FindString("Voice Volume").c_str());
	m_Buttons[3]->SetOnRelease(&VoiceVolumeButtonOnClick);

	m_Buttons[4]->SetString(STRING->FindString("User Interface Volume").c_str());
	m_Buttons[4]->SetOnRelease(&UserInterfaceVolumeButtonOnClick);

	m_Buttons[5]->SetString(STRING->FindString("Fullscreen").c_str());
	m_Buttons[5]->SetOnRelease(&FullscreenButtonOnClick);

	m_Buttons[6]->SetString(STRING->FindString("Language").c_str());
	m_Buttons[6]->SetOnRelease(&LanguageButtonOnClick);

	m_Buttons[7]->SetString(STRING->FindString("Key Bindings").c_str());
	m_Buttons[7]->SetOnRelease(&KeyBindingsButtonOnClick);

	m_Buttons[8]->SetString(STRING->FindString("Apply").c_str());
	m_Buttons[8]->SetOnRelease(&ApplyButtonOnClick);

	m_Buttons[9]->SetString(STRING->FindString("Back").c_str());
	m_Buttons[9]->SetOnRelease(&BackButtonOnClick);

	for(unsigned char i = 0; i < 10; ++i)
	{
		m_Buttons[i]->FitToText();
		if(i > 0)
			m_Buttons[i]->SetPositionY(m_Buttons[i - 1]->GetPositionY() + m_Buttons[i - 1]->GetSizeY() + 10);
		m_ObjectManager.AddObjectToLayer(0, m_Buttons[i]);
	}

	m_nSelection = static_cast<EMenu>(0);
	SetSelection(MIN);

	m_bIsChangingName = false;
	m_Profile = *PROFILE_MANAGER->GetCurrentProfile();
	m_WasFullscreen = PROFILE_MANAGER->GetCurrentProfile()->bFullscreen;
}

CState_Options::~CState_Options()
{

}

bool CState_Options::Input()
{
	if(!m_bIsChangingName)
	{
		CSGD_DirectInput* DirectInput = CSGD_DirectInput::GetInstance();
		if(DirectInput->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[UP]) || XBOX->GetController(0)->IsPressed_DPAD_UP())
		{
			SetSelection(static_cast<EMenu>(GetSelection() - 1));

			CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface")->Play();
		}
		if(DirectInput->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[DOWN]) || XBOX->GetController(0)->IsPressed_DPAD_DOWN())
		{
			SetSelection(static_cast<EMenu>(GetSelection() + 1));

			CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepA.wav", "User Interface")->Play();
		}

		if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[LEFT]) || XBOX->GetController(0)->IsPressed_DPAD_LEFT())
		{
			switch(m_nSelection)
			{
			case MUSIC_VOLUME:
				{
					m_Profile.MusicVolume = max(m_Profile.MusicVolume - 1, 0);
					CAudioManager::GetInstance()->GetCategory("Music")->SetVolume(m_Profile.MusicVolume / static_cast<float>(UCHAR_MAX));
					break;
				}
			case EFFECT_VOLUME:
				{
					m_Profile.EffectVolume = max(m_Profile.EffectVolume - 1, 0);
					if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/Punch.wav", "Effects")->IsPlaying())
						CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/Punch.wav", "Effects")->Play();
					CAudioManager::GetInstance()->GetCategory("Effects")->SetVolume(m_Profile.EffectVolume / static_cast<float>(UCHAR_MAX));
					break;
				}
			case VOICE_VOLUME:
				{
					m_Profile.VoiceVolume = max(m_Profile.VoiceVolume - 1, 0);
					CAudioManager::GetInstance()->GetCategory("Voices")->SetVolume(m_Profile.VoiceVolume / static_cast<float>(UCHAR_MAX));
					if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/BarrelRoll.wav", "Voices")->IsPlaying())
						CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/BarrelRoll.wav", "Voices")->Play();
					break;
				}
			case UI_VOLUME:
				{
					m_Profile.UserInterfaceVolume = max(m_Profile.UserInterfaceVolume - 1, 0);
					if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepB.wav", "User Interface")->IsPlaying())
						CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepB.wav", "User Interface")->Play();
					CAudioManager::GetInstance()->GetCategory("User Interface")->SetVolume(m_Profile.UserInterfaceVolume / static_cast<float>(UCHAR_MAX));
					break;
				}
			}
		}
		else if(CSGD_DirectInput::GetInstance()->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[LEFT]) || XBOX->GetController(0)->IsPressed_DPAD_LEFT())
		{
			switch(m_nSelection)
			{
			case FULLSCREEN:
				FullscreenButtonOnClick();
				break;
			case LANGUAGE:
				LanguageButtonOnClick();
				break;
			}
		}

		if(CSGD_DirectInput::GetInstance()->KeyDown(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[RIGHT]) || XBOX->GetController(0)->IsPressed_DPAD_RIGHT())
		{
			switch(m_nSelection)
			{
			case MUSIC_VOLUME:
				{
					m_Profile.MusicVolume = min(m_Profile.MusicVolume + 1, UCHAR_MAX);
					CAudioManager::GetInstance()->GetCategory("Music")->SetVolume(m_Profile.MusicVolume / static_cast<float>(UCHAR_MAX));
					break;
				}
			case EFFECT_VOLUME:
				{
					m_Profile.EffectVolume = min(m_Profile.EffectVolume + 1, UCHAR_MAX);
					if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/Punch.wav", "Effects")->IsPlaying())
						CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Units/Punch.wav", "Effects")->Play();
					CAudioManager::GetInstance()->GetCategory("Effects")->SetVolume(m_Profile.EffectVolume / static_cast<float>(UCHAR_MAX));
					break;
				}
			case VOICE_VOLUME:
				{
					m_Profile.VoiceVolume = min(m_Profile.VoiceVolume + 1, UCHAR_MAX);
					CAudioManager::GetInstance()->GetCategory("Voices")->SetVolume(m_Profile.VoiceVolume / static_cast<float>(UCHAR_MAX));
					if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/BarrelRoll.wav", "Voices")->IsPlaying())
						CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/Voices/BarrelRoll.wav", "Voices")->Play();
					break;
				}
			case UI_VOLUME:
				{
					m_Profile.UserInterfaceVolume = min(m_Profile.UserInterfaceVolume + 1, UCHAR_MAX);
					if(!CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepB.wav", "User Interface")->IsPlaying())
						CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/BeepB.wav", "User Interface")->Play();
					CAudioManager::GetInstance()->GetCategory("User Interface")->SetVolume(m_Profile.UserInterfaceVolume / static_cast<float>(UCHAR_MAX));
					break;
				}
			}
		}
		else if(CSGD_DirectInput::GetInstance()->KeyPressed(PROFILE_MANAGER->GetCurrentProfile()->unKeyBindings[RIGHT]) || XBOX->GetController(0)->IsPressed_DPAD_RIGHT())
		{
			switch(m_nSelection)
			{
			case FULLSCREEN:
				FullscreenButtonOnClick();
				break;
			case LANGUAGE:
				LanguageButtonOnClick();
				break;
			}
		}
	}
	else
	{
		char NewLetter = CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx();
		int length = strlen(m_Profile.szName);

		if(length < 31)
		{
			m_Profile.szName[length] = NewLetter;
			m_Profile.szName[length + 1] = NULL;
		}
		if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_BACK) && length > 0)
		{
			m_Profile.szName[length - 1] = '\0';
		}
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_RETURN) || XBOX->GetController(0)->IsPressed_A())
	{
		CAudioManager::GetInstance()->GetSound("Resources/Sounds/Effects/User Interface/ButtonClick.wav", "User Interface")->Play();
		switch(m_nSelection)
		{
		case NAME:
			NameButtonOnClick();
			break;
		case FULLSCREEN:
			FullscreenButtonOnClick();
			break;
		case LANGUAGE:
			LanguageButtonOnClick();
			break;
		case KEYBINDINGS:
			KeyBindingsButtonOnClick();
			break;
		case APPLY:
			ApplyButtonOnClick();
			break;
		case BACK:
			BackButtonOnClick();
			break;
		}
	}
	return true;
}

void CState_Options::Update(float _delta)
{
	m_ObjectManager.UpdateAllObjects(_delta);
}

void CState_Options::Draw()
{
	CSGD_Direct3D::GetInstance()->Clear();

	CSpriteManager::GetInstance()->GetSprite("Resources/Graphics/User Interface/FoodFight_Menu.png")->Draw(D3DXVECTOR2(0.0f, -250.0f));

	DrawGUIFrame(this, D3DXVECTOR2(100, 50), D3DXVECTOR2(1024 - 200, 768 - 100));

	D3DXMATRIX Transform;
	D3DXMatrixIdentity(&Transform);
	RECT ViewRectangle;
	ViewRectangle.left = 0;
	ViewRectangle.right = 1024;
	ViewRectangle.top = 0;
	ViewRectangle.bottom = 768;
	m_ObjectManager.DrawAllObjects(Transform, ViewRectangle);

	D3DCOLOR VolumeBarColors = D3DCOLOR_XRGB(255, 255, 0);
	float LERPValue = m_Profile.MusicVolume / 255.0f;
	CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2(500, m_Buttons[1]->GetPositionY() + m_Buttons[1]->GetSizeY() * 0.5f), D3DXVECTOR2((400 * LERPValue) + 500, m_Buttons[1]->GetPositionY() + m_Buttons[1]->GetSizeY() * 0.5f), m_Buttons[1]->GetSizeY(), VolumeBarColors);

	LERPValue = m_Profile.EffectVolume / 255.0f;
	CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2(500, m_Buttons[2]->GetPositionY() + m_Buttons[2]->GetSizeY() * 0.5f), D3DXVECTOR2((400 * LERPValue) + 500, m_Buttons[2]->GetPositionY() + m_Buttons[2]->GetSizeY() * 0.5f), m_Buttons[2]->GetSizeY(), VolumeBarColors);

	LERPValue = m_Profile.VoiceVolume / 255.0f;
	CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2(500, m_Buttons[3]->GetPositionY() + m_Buttons[3]->GetSizeY() * 0.5f), D3DXVECTOR2((400 * LERPValue) + 500, m_Buttons[3]->GetPositionY() + m_Buttons[3]->GetSizeY() * 0.5f), m_Buttons[3]->GetSizeY(), VolumeBarColors);

	LERPValue = m_Profile.UserInterfaceVolume / 255.0f;
	CSGD_Direct3D::GetInstance()->DrawLine(D3DXVECTOR2(500, m_Buttons[4]->GetPositionY() + m_Buttons[4]->GetSizeY() * 0.5f), D3DXVECTOR2((400 * LERPValue) + 500, m_Buttons[4]->GetPositionY() + m_Buttons[4]->GetSizeY() * 0.5f), m_Buttons[4]->GetSizeY(), VolumeBarColors);

	BITMAP->DrawStringSprite("Resources/Data/Font.spr", STRING->FindString("Options").c_str(), 512.0f, 100.0f);

	D3DCOLOR NameColor = m_bIsChangingName ? D3DCOLOR_XRGB(255, 127, 127) : D3DCOLOR_XRGB(255, 255, 255);
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", m_Profile.szName, m_Buttons[0]->GetPositionX() + m_Buttons[0]->GetSizeX() + 50.0f, m_Buttons[0]->GetPositionY(), 1, 1, 0, NameColor);

	D3DCOLOR LanguageColor = m_nSelection == 6 ? D3DCOLOR_XRGB(255, 127, 127) : D3DCOLOR_XRGB(255, 255, 255);

	char LanguageText[16];
	switch(m_Profile.Language)
	{
	case EL_English:
		strcpy_s(LanguageText, 16, "English");
		break;
	case EL_Spanish:
		strcpy_s(LanguageText, 16,  "Español");
		break;
	default:
		break;
	}
	BITMAP->DrawStringSprite("Resources/Data/Font.spr", LanguageText, m_Buttons[6]->GetPositionX() + m_Buttons[6]->GetSizeX() + 50.0f, m_Buttons[6]->GetPositionY(), 1, 1, 0, LanguageColor);

	BITMAP->DrawStringSprite("Resources/Data/Font.spr", m_Profile.bFullscreen ? "True" : "False", m_Buttons[5]->GetPositionX() + m_Buttons[5]->GetSizeX() + 50.0f, m_Buttons[5]->GetPositionY(), 1, 1, 0);
}

bool CState_Options::WasFullscreen()
{
	return m_WasFullscreen;
}

void CState_Options::SetIfChangingName(bool ChangingName)
{
	m_bIsChangingName = ChangingName;
}

bool CState_Options::IsChangingName()
{
	return m_bIsChangingName;
}

std::string CState_Options::GetCurrentName()
{
	return m_Profile.szName;
}

void CState_Options::SetCurrentName(const char* Name)
{
	strcpy_s(m_Profile.szName, strlen(Name), Name);
}

void CState_Options::CNameButtonOnClickFunctor::operator()()
{
	CMenuButton::operator()();
	GetOptionsState()->SetIfChangingName(!GetOptionsState()->IsChangingName());
}

void CState_Options::CFullscreenButtonOnClickFunctor::operator()()
{
	CMenuButton::operator()();
	GetOptionsState()->m_Profile.bFullscreen = !GetOptionsState()->m_Profile.bFullscreen;
}

void CState_Options::CLanguageButtonOnClickFunctor::operator()()
{
	CMenuButton::operator()();
	GetOptionsState()->m_Profile.Language = static_cast<ELanguage>(GetOptionsState()->m_Profile.Language + 1);

	if(GetOptionsState()->m_Profile.Language == EL_MAX)
		GetOptionsState()->m_Profile.Language = static_cast<ELanguage>(0);
}

void CState_Options::CKeyBindingsButtonOnClickFunctor::operator()()
{
	CMenuButton::operator()();
	CGame::GetInstance()->PushState(new CState_KeyBinding);
}

void CState_Options::CApplyButtonOnClickFunctor::operator()()
{
	CMenuButton::operator()();
	*PROFILE_MANAGER->GetCurrentProfile() = GetOptionsState()->m_Profile;

	if(PROFILE_MANAGER->GetCurrentProfile()->bFullscreen != GetOptionsState()->WasFullscreen())
		CGame::GetInstance()->ChangeDisplayParameters(1024, 768, !PROFILE_MANAGER->GetCurrentProfile()->bFullscreen);

	if(GetOptionsState()->m_Profile.Language == EL_English)
		CStringTable::GetInstance()->LoadDictionary("Resources/Data/English.txt");
	else if(GetOptionsState()->m_Profile.Language == EL_Spanish)
		CStringTable::GetInstance()->LoadDictionary("Resources/Data/Spanish.txt");

	CGame::GetInstance()->PopState();
}

void CState_Options::CBackButtonOnClickFunctor::operator()()
{
	CAudioManager::GetInstance()->GetCategory("Music")->SetVolume(PROFILE_MANAGER->GetCurrentProfile()->MusicVolume / static_cast<float>(UCHAR_MAX));
	CAudioManager::GetInstance()->GetCategory("Effects")->SetVolume(PROFILE_MANAGER->GetCurrentProfile()->EffectVolume / static_cast<float>(UCHAR_MAX));
	CAudioManager::GetInstance()->GetCategory("Voices")->SetVolume(PROFILE_MANAGER->GetCurrentProfile()->VoiceVolume / static_cast<float>(UCHAR_MAX));
	CAudioManager::GetInstance()->GetCategory("User Interface")->SetVolume(PROFILE_MANAGER->GetCurrentProfile()->UserInterfaceVolume / static_cast<float>(UCHAR_MAX));

	CGame::GetInstance()->PopState();
}

CState_Options::EMenu CState_Options::GetSelection() const
{
	return m_nSelection;
}

void CState_Options::SetSelection(CState_Options::EMenu Selection)
{
	if(m_Buttons[m_nSelection])
		m_Buttons[m_nSelection]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));

	if(Selection > MAX)
		Selection = MIN;
	else if(Selection < MIN)
		Selection = MAX;

	m_nSelection = Selection;

	if(m_Buttons[m_nSelection])
		m_Buttons[m_nSelection]->SetTextColor(D3DCOLOR_XRGB(255, 127, 127));
}

void CState_Options::CMenuButton::operator()()
{
	GetOptionsState()->SetSelection(static_cast<CState_Options::EMenu>(GetIndex()));
}

unsigned char CState_Options::CMenuButton::GetIndex() const
{
	return m_Index;
}

void CState_Options::CMenuButton::SetIndex(unsigned char Index)
{
	m_Index = Index;
}

CState_Options* CState_Options::CMenuButton::GetOptionsState() const
{
	return m_OptionsState;
}

void CState_Options::CMenuButton::SetOptionsState(CState_Options* OptionsState)
{
	m_OptionsState = OptionsState;
}