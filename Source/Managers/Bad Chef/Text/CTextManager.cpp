#include "CTextManager.h"
#include "../../../Game/Objects/Base/CCamera.h"

CTextManager CTextManager::m_Instance;

CTextManager::CTextManager(void)
{
	m_Fonts.reserve(16);
	m_FontSprites.reserve(16);
}

CTextManager::~CTextManager(void)
{

}

void CTextManager::ShutdownTextManager()
{
	m_Fonts.clear();
	m_FontSprites.clear();
}

void CTextManager::AddFont(const char* _fileName, int _cellWidth, int _cellHeight, char _startingChar, int _cols, int _rows)
{
	for(unsigned int i = 0; i < m_Fonts.size(); i++)
	{
		if(!stricmp(m_Fonts[i].GetFileName(), _fileName))
			return;
	}

	CBitmapFont newFont(_fileName,_cellWidth,_cellHeight,_startingChar,_cols,_rows);
	m_Fonts.push_back(newFont);
}

void CTextManager::AddFontSprite(const char* _fileName)
{
	for(unsigned int i = 0; i < m_FontSprites.size(); i++)
	{
		if(!stricmp(m_FontSprites[i].GetFileName(), _fileName))
			return;
	}

	// Push that shit back!!!!!!
	CBitmapFont newFontSprite(_fileName);
	m_FontSprites.push_back(newFontSprite);
}

D3DXVECTOR2 CTextManager::DrawString(const char* _fileName, const char* _string, float _posX, float _posY, float _scaleX, float _scaleY, float _rotation, D3DCOLOR _color, D3DXVECTOR2* _bounds)
{
	//D3DXVECTOR2 the text takes up!
	D3DXVECTOR2 Size;

	Size.x = 0;
	Size.y = 0;

	for(unsigned int i = 0; i < m_Fonts.size(); i++)
	{
		if(!stricmp(m_Fonts[i].GetFileName(), _fileName))
		{
			Size = m_Fonts[i].DrawString(_string,_posX,_posY,_scaleX,_scaleY,_rotation,_color,_bounds);
			break;
		}
	}

	return Size;
}

D3DXVECTOR2 CTextManager::DrawStringSprite(const char* _fileName, const char* _string, float _posX, float _posY,
							  float _scaleX, float _scaleY, float _rotation, D3DCOLOR _color, D3DXVECTOR2* _bounds)
{
	//D3DXVECTOR2 the text takes up!
	D3DXVECTOR2 Size;

	Size.x = 0;
	Size.y = 0;

	for(unsigned int i = 0; i < m_FontSprites.size(); i++)
	{
		if(!stricmp(m_FontSprites[i].GetFileName(), _fileName))
		{
			Size = m_FontSprites[i].DrawStringSprite(_string,_posX,_posY,_scaleX,_scaleY,_rotation,_color,_bounds);
			break;
		}
	}

	return Size;
}

D3DXVECTOR2 CTextManager::DrawStringWorld(const char* _fileName, const char* _string, float _posX, float _posY, float _scaleX, float _scaleY,
					float _rotation, D3DCOLOR _color, D3DXVECTOR2* _bounds)
{
	//D3DXVECTOR2 the text takes up!
	D3DXVECTOR2 Size;

	Size.x = 0;
	Size.y = 0;

	for(unsigned int i = 0; i < m_Fonts.size(); i++)
	{
		if(!stricmp(m_Fonts[i].GetFileName(), _fileName))
		{
			_posX = _posX - CAMERA->GetPositionX();
			_posY = _posY - CAMERA->GetPositionY();
			m_Fonts[i].DrawString(_string,_posX,_posY,_scaleX,_scaleY,_rotation,_color,_bounds);
			break;
		}
	}

	return Size;
}

D3DXVECTOR2 CTextManager::DrawStringSpriteWorld(const char* _fileName, const char* _string, float _posX, float _posY, float _scaleX, float _scaleY,
					float _rotation, D3DCOLOR _color, D3DXVECTOR2* _bounds)
{
	//D3DXVECTOR2 the text takes up!
	D3DXVECTOR2 Size;
	Size.x = 0;
	Size.y = 0;

	for(unsigned int i = 0; i < m_FontSprites.size(); ++i)
	{
		if(!stricmp(m_FontSprites[i].GetFileName(), _fileName))
		{
			_posX = _posX - CAMERA->GetPositionX();
			_posY = _posY - CAMERA->GetPositionY();
			Size = m_FontSprites[i].DrawStringSprite(_string,_posX,_posY,_scaleX,_scaleY,_rotation,_color,_bounds);
			break;
		}
	}

	return Size;
}

D3DXVECTOR2 CTextManager::CalculateSize(const char* _fileName, const char* _string, float _scaleX, float _scaleY, D3DXVECTOR2* _bounds)
{
	//D3DXVECTOR2 the text takes up!
	D3DXVECTOR2 Size;
	Size.x = 0;
	Size.y = 0;

	for(unsigned int i = 0; i < m_FontSprites.size(); ++i)
	{
		if(!stricmp(m_FontSprites[i].GetFileName(), _fileName))
		{
			Size = m_FontSprites[i].CalculateD3DXVECTOR2(_string, _scaleX, _scaleY, _bounds);
			break;
		}
	}

	return Size;
}

D3DXVECTOR2 CTextManager::CalculateSizeSprite(const char* _fileName, const char* _string, float _scaleX, float _scaleY, D3DXVECTOR2* _bounds)
{
	//D3DXVECTOR2 the text takes up!
	D3DXVECTOR2 Size;
	Size.x = 0;
	Size.y = 0;

	for(unsigned int i = 0; i < m_FontSprites.size(); ++i)
	{
		if(!stricmp(m_FontSprites[i].GetFileName(), _fileName))
		{
			Size = m_FontSprites[i].CalculateD3DXVECTOR2Sprite(_string, _scaleX, _scaleY, _bounds);
			break;
		}
	}

	return Size;
}