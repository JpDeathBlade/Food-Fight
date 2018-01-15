#ifndef FF_JS_CBITMAPFONT_H_
#define FF_JS_CBITMAPFONT_H_

#include <windows.h>
#include <d3d9.h>
#include "..\Sprites\CSpriteManager.h"

#define SPRITE CSpriteManager::GetInstance()

class CBitmapFont
{
private:
	char*	m_szFontFileName;
	int		m_nCellWidth;
	int		m_nCellHeight;
	char	m_cCellStartingChar;
	int		m_nNumCols;
	int		m_nNumRows;

private:
	RECT CellAlgorithm(int _cellID);

public:
	CBitmapFont(const char* _fileName, int _cellWidth, int _cellHeight, char _startingChar, int _cols, int _rows);
	CBitmapFont(const char* _fileName);
	CBitmapFont(const CBitmapFont &ref);
	CBitmapFont &operator= (const CBitmapFont &ref);
	~CBitmapFont();

	void DrawLetter(const char  _letter, float _posX, float _posY, float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, D3DCOLOR _color = D3DCOLOR_XRGB(255,255,255));

	D3DXVECTOR2 DrawString(const char* _string, float _posX, float _posY, float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, D3DCOLOR _color = D3DCOLOR_XRGB(255,255,255), D3DXVECTOR2* _bounds = NULL);

	D3DXVECTOR2 DrawStringSprite(const char* _string, float _posX, float _posY, float _scaleX = 1.0f, float _scaleY = 1.0f, float _rotation = 0.0f, D3DCOLOR _color = D3DCOLOR_XRGB(255,255,255), D3DXVECTOR2* _bounds = NULL);

	char*	GetFileName()			{ return m_szFontFileName;	  }
	int		GetCellWidth()			{ return m_nCellWidth;		  }
	int		GetCellHeight()			{ return m_nCellHeight;		  }
	int		GetNumCols()			{ return m_nNumCols;		  }
	int		GetNumRows()			{ return m_nNumRows;		  }
	char	GetCellStartingChar()	{ return m_cCellStartingChar; }

	void	SetFileName(const char* _fileName)			{ m_szFontFileName = const_cast<char*>(_fileName);	}
	void	SetCellWidth(int _cellWidth)				{ m_nCellWidth = _cellWidth;		   }
	void	SetCellHeight(int _cellHeight)				{ m_nCellHeight = _cellHeight;		   }
	void	SetNumCols(int _cols)						{ m_nNumCols = _cols;				   }
	void	SetNumRows(int _rows)						{ m_nNumRows = _rows;				   }
	void	SetCellStartingChar(char _startingChar)		{ m_cCellStartingChar = _startingChar; }

	D3DXVECTOR2 CalculateD3DXVECTOR2(const char* _string, float _scaleX = 1.0f, float _scaleY = 1.0f, D3DXVECTOR2* _bounds = NULL);
	D3DXVECTOR2 CalculateD3DXVECTOR2Sprite(const char* _string, float _scaleX = 1.0f, float _scaleY = 1.0f, D3DXVECTOR2* _bounds = NULL);
};
#endif