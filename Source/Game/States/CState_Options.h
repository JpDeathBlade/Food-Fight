#ifndef FF_JS_CSTATE_OPTIONS_H_
#define FF_JS_CSTATE_OPTIONS_H_

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"
#include "../Objects/Base/IAnimable.h"
#include "../../Managers/Bad Chef/Profiles/CProfileManager.h"
#include <string>

class CButton;

class CState_Options : public IGameState, public IAnimable
{
	enum EMenu { MIN, NAME = MIN, MUSIC_VOLUME, EFFECT_VOLUME, VOICE_VOLUME, UI_VOLUME, FULLSCREEN, LANGUAGE, KEYBINDINGS, APPLY, BACK, MAX = BACK};

	class CMenuButton : public IFunctor
	{
		CState_Options* m_OptionsState;
		unsigned char m_Index;
	public:
		virtual void operator()();
		CState_Options* GetOptionsState() const;
		void SetOptionsState(CState_Options* OptionsState);

		unsigned char GetIndex() const;
		void SetIndex(unsigned char Index);
	};

	class CNameButtonOnClickFunctor : public CMenuButton
	{
	public:
		void operator()();
	} NameButtonOnClick;

	CMenuButton MusicVolumeButtonOnClick;
	CMenuButton EffectsVolumeButtonOnClick;
	CMenuButton UserInterfaceVolumeButtonOnClick;
	CMenuButton VoiceVolumeButtonOnClick;

	class CFullscreenButtonOnClickFunctor : public CMenuButton
	{
	public:
		void operator()();
	} FullscreenButtonOnClick;

	class CLanguageButtonOnClickFunctor : public CMenuButton
	{
	public:
		void operator()();
	} LanguageButtonOnClick;

	class CKeyBindingsButtonOnClickFunctor : public CMenuButton
	{
	public:
		void operator()();
	} KeyBindingsButtonOnClick;

	class CApplyButtonOnClickFunctor : public CMenuButton
	{
	public:
		void operator()();
	} ApplyButtonOnClick;

	class CBackButtonOnClickFunctor : public CMenuButton
	{
	public:
		void operator()();
	} BackButtonOnClick;

	CButton* m_Buttons[10];
	EMenu m_nSelection;
	
	EMenu GetSelection() const;
	void SetSelection(EMenu Selection);

	bool m_bIsChangingName;
	bool m_WasFullscreen;
public:
	tProfile m_Profile;

	CState_Options(); // Default Constructor
	CState_Options(const CState_Options& Source); // Copy Constructor - Disabled
	CState_Options& operator=(const CState_Options& Source); // Assignment Operator - Disabled
	~CState_Options(); // Destructor

	bool Input();
	void Update(float _delta);
	void Draw();

	bool WasFullscreen();
	void SetIfChangingName(bool ChangingName);
	bool IsChangingName();

	std::string GetCurrentName();
	void SetCurrentName(const char* Name);
};
#endif