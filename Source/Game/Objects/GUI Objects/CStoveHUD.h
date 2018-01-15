#ifndef _CStoveHUD_H_
#define _CStoveHUD_H_

#include "CGUIObject.h"
#include "CButton.h"
#include "CursorInterfaces.h"

class CSprite;
class CSound;
class CPan;

class CSmokeDetectorButton : public CButton
{
	bool CheckIfPanIsBurning(CPan* Pan);
public:
	CSmokeDetectorButton();
	CSmokeDetectorButton(const CSmokeDetectorButton& Source);
	CSmokeDetectorButton& operator=(const CSmokeDetectorButton& Source);
	~CSmokeDetectorButton();

	void Update(float Delta);
	void Draw();

	void OnClick();
};

class CStove;

class CStoveHUD : public CGUIObject, public IClickable
{
	static CStoveHUD* m_Instance;
	D3DXVECTOR2 m_Size;
	CSmokeDetectorButton m_SmokeDetectorButton;
	CSprite* m_FruitSprite;
	CSprite* m_MeatSprite;
	CSprite* m_EggSprite;
	CSprite* m_WheatSprite;
	CStove* m_TargetStove;

	CStoveHUD();
	CStoveHUD(const CStoveHUD& Source);
	CStoveHUD& operator=(const CStoveHUD& Source);
	~CStoveHUD();
public:
	static CStoveHUD* GetInstance();
	static void DeleteInstance();

	void Update(float Delta);
	void Draw();
	RECT GetDrawRectangle() const;
	RECT GetCollisionRectangle() const;

	void OnClick();

	void SetFruitSprite(CSprite* FruitSprite);
	void SetMeatSprite(CSprite* MeatSprite);
	void SetEggSprite(CSprite* EggSprite);
	void SetWheatSprite(CSprite* WheatSprite);

	CStove* GetTargetStove() const;
	void SetTargetStove(CStove* Stove);

	CSmokeDetectorButton* GetSmokeDetectorButton();

	static IObject* CreateStoveHUD(CObjectManager* ObjectManager);
};
#endif