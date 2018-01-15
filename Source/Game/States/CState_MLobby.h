//////////////////////////////////////////////////////
// File: "CState_MLobby.h"
// Creator: MG
// Creation Date: 9/28/09
// Last Modified: 9/28/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#ifndef _CMLobby_H_
#define _CMLobby_H_
#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"
#include "../Objects/Base/IAnimable.h"

class CButton;

class CState_MLobby : public IGameState, public IAnimable
{
	enum EMPLobby { START, CHALLENGING, CHALLENGED };
	enum EMenu { MIN, CHALLENGE = MIN, BACK, MAX = BACK};

	EMenu m_nSelection;
	
	EMenu GetSelection() const;
	void SetSelection(EMenu Selection);

	char m_Challenger[32];
public:
	CState_MLobby(); // Default Constructor
	CState_MLobby(const CState_MLobby& Source); // Copy Constructor - Disabled
	CState_MLobby& operator=(const CState_MLobby& Source); // Assignment Operator - Disabled
	~CState_MLobby(); // Destructor

	class CChallengeButtonOnClickFunctor : public IFunctor
	{
		CState_MLobby* m_Lobby;
	public:
		void operator()();

		CState_MLobby* GetLobby() const;
		void SetLobby(CState_MLobby* Lobby);
	} ChallengeButtonOnClick;

	class CAcceptButtonOnClickFunctor : public IFunctor
	{
		CState_MLobby* m_Lobby;
	public:
		void operator()();

		CState_MLobby* GetLobby() const;
		void SetLobby(CState_MLobby* Lobby);
	} AcceptButtonOnClick;

	class CBackButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} BackButtonOnClick;

	class CDeclineButtonOnClickFunctor : public IFunctor
	{
		CState_MLobby* m_Lobby;
	public:
		void operator()();

		CState_MLobby* GetLobby() const;
		void SetLobby(CState_MLobby* Lobby);
	} DeclineButtonOnClick;

	bool m_IsTypingIP;
	char m_MyIP[24];
	char m_TypedIP[24];
	EMPLobby m_SubState;
	CButton* m_Buttons[2];

	void OnEnter();

	bool Input();
	void Update(float Delta);
	void Draw();
};
#endif