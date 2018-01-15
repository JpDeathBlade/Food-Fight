#ifndef _CButton_H_
#define _CButton_H_
#include "CGUIObject.h"
#include "CursorInterfaces.h"
#include <string>
#include <d3dx9.h>

class IFunctor;

class CButton : public CGUIObject, public IClickable, public IHoverable
{
	D3DXVECTOR2 m_Size;
	D3DXVECTOR2 m_FontScale;
	D3DXVECTOR2 m_StringOffset;
	std::string m_String;
	std::string m_FontFileName;
	D3DCOLOR m_TextColor;
	bool m_Enabled;
	bool m_FitTextToSize;

	IFunctor* m_OnClick;
	IFunctor* m_OnRelease;
	IFunctor* m_OnHover;
public:
	CButton();
	CButton(const CButton& Source);
	virtual CButton& operator=(const CButton& Source);
	virtual ~CButton();

	void Draw();
	RECT GetDrawRectangle() const;
	RECT GetCollisionRectangle() const;

	D3DXVECTOR2 GetSize() const;
	float GetSizeX() const;
	float GetSizeY() const;

	void SetSize(D3DXVECTOR2 Size);
	void SetSize(float X, float Y);
	void SetSizeX(float X);
	void SetSizeY(float Y);

	D3DXVECTOR2 GetFontScale() const;
	float GetFontScaleX() const;
	float GetFontScaleY() const;

	void SetFontScale(D3DXVECTOR2 Scale);
	void SetFontScale(float X, float Y);
	void SetFontScaleX(float X);
	void SetFontScaleY(float Y);

	std::string GetString() const;
	void SetString(std::string String);

	std::string GetFontFileName() const;
	void SetFontFileName(std::string FontFileName);

	D3DXVECTOR2 GetStringOffset() const;
	float GetStringOffsetX() const;
	float GetStringOffsetY() const;

	void SetStringOffset(D3DXVECTOR2 StringOffset);
	void SetStringOffset(float X, float Y);
	void SetStringOffsetX(float X);
	void SetStringOffsetY(float Y);

	D3DCOLOR GetTextColor() const;
	void SetTextColor(D3DCOLOR Color);

	bool IsEnabled() const;
	virtual void Enable();
	virtual void Disable();

	virtual void OnClick();
	void SetOnClick(IFunctor* OnClick);
	IFunctor* GetOnClick() { return m_OnClick; }

	virtual void OnRelease();
	void SetOnRelease(IFunctor* OnRelease);
	IFunctor* GetOnRelease() { return m_OnRelease; }

	virtual void OnHover();
	void SetOnHover(IFunctor* OnHover);
	IFunctor* GetOnHover() { return m_OnHover; }

	void FitToText(int border = 0);
	void FitTextToSize(bool Fit);
};
#endif