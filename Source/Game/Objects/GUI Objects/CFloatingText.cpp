/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BUG-019 & BUG-020
// BUG FIX START
/////////////////////////////////////////////////////////////////

#include "CFloatingText.h"
#include "../../../Managers/Bad Chef/CObjectManager.h"
#include "../../../Managers/Bad Chef/Text/CTextManager.h"


void CFloatingText::Update(float Delta)
{
	if(m_fLife <= 0)
		DoRemove();

	m_vPosition += m_vVelocity * Delta;

	if(!m_bIsFlashing)
		m_fCurrentAlpha -= m_fAlphaDepreciation * Delta;
	else
		m_fCurrentAlpha -= 50;

	if(!m_bIsFlashing)
	{
		if(m_fCurrentAlpha < 0.0f)
			m_fCurrentAlpha = 0.0f;
	}

	m_fLife -= Delta;
}

void CFloatingText::Draw(void)
{
	unsigned char R = (unsigned char)(255 * m_cColor.r);
	unsigned char G = (unsigned char)(255 * m_cColor.g);
	unsigned char B = (unsigned char)(255 * m_cColor.b);

	CTextManager::GetInstance()->DrawStringSpriteWorld("Resources/Data/Font.spr", m_szText.c_str(), m_vPosition.x, m_vPosition.y, m_fScale, m_fScale, m_fRotation, D3DCOLOR_ARGB(((unsigned char)m_fCurrentAlpha), R, G, B));
}

RECT CFloatingText::GetDrawRectangle(void) const
{
	RECT drawRect;

	drawRect.left = (LONG)m_vPosition.x;
	drawRect.top = (LONG)m_vPosition.y;
	drawRect.right = drawRect.left + 10;
	drawRect.bottom = drawRect.top + 10;

	return drawRect;
}

void CFloatingText::SpawnFloatingText(CObjectManager* pManager, const char* szText, D3DXVECTOR2 vPosition, D3DXVECTOR2 vVelocity, float fLife, float fScale, float fRotation, D3DXCOLOR cColor, bool bIsFlashing)
{
	CFloatingText* newFloating = new CFloatingText();

	newFloating->m_szText = szText;
	newFloating->m_vPosition = vPosition;
	newFloating->m_vVelocity = vVelocity;
	newFloating->SetCollideable(false);
	newFloating->m_fLife = fLife;
	newFloating->m_fScale = fScale;
	newFloating->m_fRotation = fRotation;
	newFloating->m_cColor = cColor;
	newFloating->m_fCurrentAlpha = 255.0f;
	newFloating->m_fAlphaDepreciation = 255.0f/newFloating->m_fLife;
	newFloating->m_bIsFlashing = bIsFlashing;

	pManager->AddObjectToLayer(3, newFloating);
}

///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BUG-019 & BUG-020
//////////////////////////////////////////////////////////////////////////

