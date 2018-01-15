//////////////////////////////////////////////////////
// File: "CState_LevelSelect.h"
// Creator: MG
// Creation Date: 10/12/09
// Last Modified: 10/12/09
// Last Modified By: MG
//////////////////////////////////////////////////////
#ifndef CSTATE_LEVELSELECT_H_
#define CSTATE_LEVELSELECT_H_

#include "IGameState.h"
#include "../Objects/Base/IFunctor.h"

class CButton;

class CState_LevelSelect : public IGameState
{
	enum EMenu { MIN, LEVELA = MIN, LEVELB, LEVELC, BACK, MAX = BACK };

	class CLoadLevelFunctor : public IFunctor
	{
	public:
		std::string filename;
		void operator()();
	};

	class CBackButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} BackButtonOnClick;

	CLoadLevelFunctor m_Level1Load;
	CLoadLevelFunctor m_Level2Load;
	CLoadLevelFunctor m_Level3Load;

	std::string* m_FileString;

	CButton* m_Buttons[4];
	EMenu m_nSelection;
	
	EMenu GetSelection() const;
	void SetSelection(EMenu Selection);
public:
	CState_LevelSelect(); // Default Constructor
	CState_LevelSelect(const CState_LevelSelect& Source); // Copy Constructor - Disabled
	CState_LevelSelect& operator=(const CState_LevelSelect& Source); // Assignment Operator - Disabled
	~CState_LevelSelect(); // Destructor

	bool Input();
	void Update(float Delta);
	void Draw();
};

#endif