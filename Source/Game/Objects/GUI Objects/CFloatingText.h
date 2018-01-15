/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BUG-019 & BUG-020
// BUG FIX START
/////////////////////////////////////////////////////////////////
#ifndef CFLOATINGTEXT_H_
#define CFLOATINGTEXT_H_

#include "../Base/IObject.h"
#include <string>
using namespace std;

class CObjectManager;

class CFloatingText : public IObject
{
private:
	string			m_szText;
	D3DXVECTOR2		m_vPosition;
	D3DXVECTOR2		m_vVelocity;
	D3DXCOLOR		m_cColor;
	float			m_fScale;
	float			m_fRotation;
	float			m_fLife;
	float			m_fAlphaDepreciation;
	float			m_fCurrentAlpha;
	bool			m_bIsFlashing;

	CFloatingText(void) {}
	CFloatingText(CFloatingText& obj) {}
	CFloatingText& operator = (CFloatingText& obj) {}
	~CFloatingText(void) {}

public:

	void Update(float Delta);

	void Draw(void);

	RECT GetDrawRectangle(void) const;

	float GetZDepth(void) const { return m_vPosition.y; }

	static void SpawnFloatingText(CObjectManager* pManager, const char* szText, D3DXVECTOR2 vPosition, D3DXVECTOR2 vVelocity, float fLife, float fScale = 1.0f, float fRotation = 0.0f, D3DXCOLOR cColor = D3DCOLOR_XRGB(255, 255, 255), bool bIsFlashing = true);

	string GetText(void) const					{ return m_szText; }
	void SetText(const char* szText)			{ m_szText = szText; }

	D3DXVECTOR2 GetPosition(void) const			{ return m_vPosition; }
	void SetPosition(D3DXVECTOR2 vPosition)		{ m_vPosition = vPosition; }

	D3DXVECTOR2 GetVelocity(void) const			{ return m_vVelocity; }
	void SetVelocity(D3DXVECTOR2 vVelocity)		{ m_vVelocity = vVelocity; }

	D3DXCOLOR GetColor(void) const				{ return m_cColor; }
	void SetColor(D3DXCOLOR cColor)				{ m_cColor = cColor; }

	float GetScale(void) const					{ return m_fScale; }
	void SetScale(float fScale)					{ m_fScale = fScale; }

	float GetRotation(void) const				{ return m_fRotation; }
	void SetRotation(float fRotation)			{ m_fRotation = fRotation; }

	float GetLife(void) const					{ return m_fLife; }
	void SetLife(float fLife)					{ m_fLife = fLife; }
};

#endif

///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BUG-019 & BUG-020
//////////////////////////////////////////////////////////////////////////
