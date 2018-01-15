#ifndef FF_JS_CSTATE_MAINMENU_H_
#define FF_JS_CSTATE_MAINMENU_H_

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"

class CStringTable;
class CButton;

class CState_MainMenu : public IGameState
{
	enum EMenu { MIN, CAMPAIGN = MIN, MULTIPLAYER, OPTIONS, PROFILE, CREDITS, EXIT, MAX = EXIT};

	class CCampaignButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} CampaignButtonOnClick;
	
	class CMultiplayerButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} MultiplayerButtonOnClick;

	class COptionsButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} OptionsButtonOnClick;

	class CProfilesButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} ProfilesButtonOnClick;

	class CCreditsButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} CreditsButtonOnClick;

	class CExitButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} ExitButtonOnClick;

	CButton* m_Buttons[6];
	CStringTable* m_StringTable;
	EMenu m_nSelection;
	
	EMenu GetSelection() const;
	void SetSelection(EMenu Selection);
public:
	CState_MainMenu(); // Default Constructor
	CState_MainMenu(const CState_MainMenu& Source); // Copy Constructor - Disabled
	CState_MainMenu& operator=(const CState_MainMenu& Source); // Assignment Operator - Disabled
	~CState_MainMenu(); // Destructor

	bool Input();
	void Update(float Delta);
	void Draw();

	void OnEnter();
};
#endif