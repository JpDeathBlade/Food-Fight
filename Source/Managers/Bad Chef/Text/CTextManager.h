#ifndef FF_JS_CTEXTMANAGER_H_
#define FF_JS_CTEXTMANAGER_H_

#include <vector>
using std::vector;

#include "CBitmapFont.h"

// Camera MACRO to return the Camera Instance
#define BITMAP	CTextManager::GetInstance()

class CTextManager
{
private:
	vector<CBitmapFont> m_Fonts;
	vector<CBitmapFont> m_FontSprites;
	static CTextManager m_Instance;		// Instance of the CTextManager

private:
	CTextManager(void);
	CTextManager(CTextManager &ref);
	CTextManager &operator= (CTextManager &ref);

public:
	~CTextManager();

	static CTextManager* GetInstance(void) { return &m_Instance; }

	bool InitTextManager() { return true; }
	void ShutdownTextManager();

	void AddFont(const char* _fileName, int _cellWidth, int _cellHeight, char _startingChar, int _cols, int _rows);
	void AddFontSprite(const char* _fileName);

	D3DXVECTOR2 DrawString(const char* _fileName, const char* _string, float _posX, float _posY, float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, D3DCOLOR _color = D3DCOLOR_XRGB(255,255,255), D3DXVECTOR2* _bounds = NULL);

	D3DXVECTOR2 DrawStringSprite(const char* _fileName, const char* _string, float _posX, float _posY, float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, D3DCOLOR _color = D3DCOLOR_XRGB(255,255,255), D3DXVECTOR2* _bounds = NULL);

	D3DXVECTOR2 DrawStringWorld(const char* _fileName, const char* _string, float _posX, float _posY, float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, D3DCOLOR _color = D3DCOLOR_XRGB(255,255,255), D3DXVECTOR2* _bounds = NULL);

	D3DXVECTOR2 DrawStringSpriteWorld(const char* _fileName, const char* _string, float _posX, float _posY, float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, D3DCOLOR _color = D3DCOLOR_XRGB(255,255,255), D3DXVECTOR2* _bounds = NULL);

	D3DXVECTOR2 CalculateSize(const char* _fileName, const char* _string, float _scaleX = 1.0f, float _scaleY = 1.0f, D3DXVECTOR2* _bounds = NULL);
	D3DXVECTOR2 CalculateSizeSprite(const char* _fileName, const char* _string, float _scaleX = 1.0f, float _scaleY = 1.0f, D3DXVECTOR2* _bounds = NULL);
};

#endif