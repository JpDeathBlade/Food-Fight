#ifndef _CActionsHUD_H_
#define _CActionsHUD_H_
#include "CGUIObject.h"
#include "CButton.h"
#include "CursorInterfaces.h"
#include "../Base/IFunctor.h"
#include "../../../Managers/Bad Chef/Events/CEvent.h"

class CCursor;

class CActionButton : public CButton
{
	CSprite* m_Icon;
	bool m_DrawBack;
public:
	static const unsigned char Width = 60;
	static const unsigned char Height = 60;

	CActionButton(); // Default Constructor
	CActionButton(const CActionButton& Source); // Copy Constructor - Disabled
	CActionButton& operator=(const CActionButton& Source); // Assignment Operator - Disabled
	~CActionButton(); // Destructor

	void Draw();

	void SetDrawBack(bool _set) { m_DrawBack = _set; }

	CSprite* GetIcon() const;
	bool GetDrawBack() { return m_DrawBack; }
	void SetIcon(CSprite* const Icon);
};

class IObject;
class CObjectManager;
class CSelectionHUD;

class CActionsHUD : public CGUIObject, public IClickable, public IHoverable
{
	static CActionsHUD* m_Instance;
	CActionButton m_ActionButtons[6];

	class CMoveCursorFunctor : public IFunctor
	{
		CCursor* Cursor;
	public:
		CMoveCursorFunctor(); // Default Constructor
		CMoveCursorFunctor(const CMoveCursorFunctor& Source); // Copy Constructor - Disabled
		CMoveCursorFunctor& operator=(const CMoveCursorFunctor& Source); // Assignment Operator - Disabled
		~CMoveCursorFunctor(); // Destructor

		void operator()();
	} MoveCursorOnClick;

	class CAttackCursorFunctor : public IFunctor
	{
		CCursor* Cursor;
	public:
		CAttackCursorFunctor(); // Default Constructor
		CAttackCursorFunctor(const CAttackCursorFunctor& Source); // Copy Constructor - Disabled
		CAttackCursorFunctor& operator=(const CAttackCursorFunctor& Source); // Assignment Operator - Disabled
		~CAttackCursorFunctor(); // Destructor

		void operator()();
	} AttackCursorOnClick;

	class OnSubSelectionChangeFunctor : public CEventFunctor<CSelectionHUD>
	{
		void operator()();
	} OnSubSelectionChange;

	CActionsHUD(); // Default Constructor
	CActionsHUD(const CActionsHUD& Source); // Copy Constructor - Disabled
	CActionsHUD& operator=(const CActionsHUD& Source); // Assignment Operator - Disabled
	~CActionsHUD(); // Destructor
public:
	class CMoveButtonOnClickFunctor : public IFunctor
	{
		CActionButton* m_ActionButton;
	public:
		void operator()();

		void SetActionButton(CActionButton* ActionButton);
	} MoveButtonOnClick;

	class CAttackButtonOnClickFunctor : public IFunctor
	{
		CActionButton* m_ActionButton;
	public:
		void operator()();

		void SetActionButton(CActionButton* ActionButton);
	} AttackButtonOnClick;

	class CStopButtonOnClickFunctor : public IFunctor
	{
	public:
		void operator()();
	} StopButtonOnClick;

	static const unsigned short Width = 256;
	static const unsigned short Height = 128;

	static CActionsHUD* GetInstance();
	static void DeleteInstance();

	void Update(float Delta);
	void Draw();
	RECT GetDrawRectangle() const;
	RECT GetCollisionRectangle() const;

	void OnClick();
	void OnRelease();

	void OnBeginHover();
	void OnHover();
	void OnEndHover();

	static IObject* CreateActionsHUD(CObjectManager* ObjectManager);
};
#endif