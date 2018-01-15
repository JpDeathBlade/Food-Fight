#include "CBitmapFont.h"
#include "..\Sprites\CSprite.h"

CBitmapFont::CBitmapFont(const char* _fileName, int _cellWidth, int _cellHeight, 
						 char _startingChar, int _cols, int _rows)
{
	SetFileName(_fileName);
	SetCellWidth(_cellWidth);	SetCellHeight(_cellHeight);
	SetNumCols(_cols);			SetNumRows(_rows);
	SetCellStartingChar(_startingChar);
}

CBitmapFont::CBitmapFont(const char* _fileName)
{
	m_szFontFileName = const_cast<char*>(_fileName);
	m_nCellWidth = 0;
	m_nCellHeight = 0;
	m_cCellStartingChar = 0;
	m_nNumCols = 0;
	m_nNumRows = 0;
}

CBitmapFont::CBitmapFont(const CBitmapFont &ref)
{
	SetFileName(ref.m_szFontFileName);
	SetCellWidth(ref.m_nCellWidth);	SetCellHeight(ref.m_nCellHeight);
	SetNumCols(ref.m_nNumCols);		SetNumRows(ref.m_nNumRows);
	SetCellStartingChar(ref.m_cCellStartingChar);
}

CBitmapFont& CBitmapFont:: operator= (const CBitmapFont &ref)
{
	SetFileName(ref.m_szFontFileName);
	SetCellWidth(ref.m_nCellWidth);	SetCellHeight(ref.m_nCellHeight);
	SetNumCols(ref.m_nNumCols);		SetNumRows(ref.m_nNumRows);
	SetCellStartingChar(ref.m_cCellStartingChar);

	return *this;
}

CBitmapFont::~CBitmapFont()
{

}

void CBitmapFont::DrawLetter(const char _letter, float _posX, float _posY, float _scaleX, float _scaleY,
							 float _rotation, D3DCOLOR _color)
{
	// Get the sprite for the bitmap
	CSprite* Sprite = SPRITE->GetSprite(m_szFontFileName);
	// If NULL break
	if(!Sprite)
		return;

	//Get the ID of the letter
	int id = (unsigned char)(_letter) - m_cCellStartingChar;
	//Get Rect for the cell
	RECT rLetter = CellAlgorithm(id);
	//Draw that Letter
	Sprite->DrawBitmap(_posX,_posY,_scaleX,_scaleY,&rLetter,(m_nCellWidth/2.0f),(m_nCellHeight/2.0f),
		_rotation, _color);
}

D3DXVECTOR2 CBitmapFont::DrawString(const char* _string, float _posX, float _posY, float _scaleX, float _scaleY, float _rotation, D3DCOLOR _color, D3DXVECTOR2* _bounds)
{
	int id = 0;		// ID of the letter (Char value: A = 65)
	RECT rLetter;	// Draw Rect for the letter

	D3DXVECTOR2 rSize;
	rSize.x = 0;
	rSize.y = 0;

	D3DXVECTOR2 NextCharacterPosition = D3DXVECTOR2(_posX, _posY);

	// Get the sprite for the bitmap
	CSprite* Sprite = SPRITE->GetSprite(m_szFontFileName);
	// If NULL break
	if(!Sprite)
		return rSize;

	// If there is a string
	if(_string)
	{
		//Iterate through it
		for(unsigned int i = 0; i < strlen(_string); ++i)
		{
			if(_string[i] == '\n')
			{
				NextCharacterPosition.x = _posX;
				NextCharacterPosition.y += 32 * _scaleY;
			}
			else
			{
				//Get the ID of the letter
				id = static_cast<unsigned char>(_string[i]) - m_cCellStartingChar;

				//Get the Cell
				rLetter = CellAlgorithm(id);

				// Update the occupied area of the text that is about to draw.
				rSize.x = max(rSize.x, NextCharacterPosition.x + (rLetter.right - rLetter.left) * _scaleX - _posX);
				rSize.y = max(rSize.y, NextCharacterPosition.y + (rLetter.bottom - rLetter.top) * _scaleY - _posY);

				// Draw the text at the position given.
				Sprite->DrawBitmap(NextCharacterPosition.x, NextCharacterPosition.y, _scaleX, _scaleY, &rLetter, (rLetter.right - rLetter.left) * 0.5f, (rLetter.bottom - rLetter.top) * 0.5f, _rotation, _color);
				NextCharacterPosition.x += (rLetter.right - rLetter.left) * _scaleX;

				if(_bounds && _string[i] == ' ' || _string[i] == '\n')
				{
					float wordLength = 0;
					unsigned int j = i;
					RECT nextLetterRect;
					while(j < strlen(_string))
					{
						j++;
						if(_string[j] == ' ' || _string[j] == '\n')
							break;
						else
						{
							nextLetterRect = CellAlgorithm(static_cast<unsigned char>(_string[j]) - m_cCellStartingChar);
							wordLength += nextLetterRect.right - nextLetterRect.left;
						}
					}

					if((NextCharacterPosition.x + wordLength * _scaleX) - _posX > _bounds->x) // Out of the right bounds, new line.
					{
						NextCharacterPosition.x = _posX;
						NextCharacterPosition.y += 32 * _scaleY;
					}

					if((NextCharacterPosition.y + (rLetter.bottom - rLetter.top) * _scaleY) - _posY > _bounds->y) // Out of the bottom bounds, should not draw.
					{
						return rSize;
					}
				}
			}
		}
	}

	return rSize;
}

D3DXVECTOR2 CBitmapFont::DrawStringSprite(const char* _string, float _posX, float _posY, float _scaleX, float _scaleY, float _rotation, D3DCOLOR _color, D3DXVECTOR2* _bounds)
{
	int id = 0;		// ID of the letter (Char value: A = 65)
	RECT rLetter;	// Draw Rect for the letter

	D3DXVECTOR2 rSize;
	rSize.x = 0;
	rSize.y = 0;

	D3DXVECTOR2 NextCharacterPosition = D3DXVECTOR2(_posX, _posY);

	// Get the sprite for the bitmap
	CSprite* Sprite = SPRITE->GetSprite(m_szFontFileName);
	// If NULL break
	if(!Sprite)
		return rSize;


	//loop through the text 1 character at a time
	for(unsigned int i = 0; i < strlen(_string); i++)
	{
		// If newline
		if(_string[i] == '\n')
		{
				NextCharacterPosition.x = _posX;
				NextCharacterPosition.y += 32 * _scaleY + 4;
		}
		else
		{
			id = static_cast<unsigned char>(_string[i]) - m_cCellStartingChar;

			char buffer[8];
			_itoa_s(id, buffer, 8, 10);

			// Get the Draw Rectangle of the letter
			rLetter = Sprite->GetAnimation(0)->GetFrame(buffer)->GetDrawRectangle();

			// Update the occupied area of the text that is about to draw.
			rSize.x = max(rSize.x, NextCharacterPosition.x + (rLetter.right - rLetter.left) * _scaleX - _posX);
			rSize.y = max(rSize.y, NextCharacterPosition.y + (rLetter.bottom - rLetter.top) * _scaleY - _posY);

			// Draw the text at the position given.
			Sprite->DrawBitmap(NextCharacterPosition.x, NextCharacterPosition.y, _scaleX, _scaleY, &rLetter, (rLetter.right - rLetter.left) * 0.5f, (rLetter.bottom - rLetter.top) * 0.5f, _rotation, _color);
			NextCharacterPosition.x += (rLetter.right - rLetter.left) * _scaleX;
/////////////////////////////////////////////////////////////////
// BUG FIX
// Reference Bug # BUG-001
// BUG FIX START
/////////////////////////////////////////////////////////////////
			if(_bounds && _string[i] == ' ' || _string[i] == '\n')
			{
				float wordLength = 0;
				unsigned int j = i;
				RECT nextLetterRect;
				while(j < strlen(_string))
				{
					j++;
					if(_string[j] == ' ' || _string[j] == '\n')
						break;
					else
					{
						id = static_cast<unsigned char>(_string[j]) - m_cCellStartingChar;
						char buffer[8];
						_itoa_s(id, buffer, 8, 10);
						nextLetterRect = Sprite->GetAnimation(0)->GetFrame(buffer)->GetDrawRectangle();
						wordLength += nextLetterRect.right - nextLetterRect.left;
					}
				}

				if((NextCharacterPosition.x + wordLength * _scaleX) - _posX > _bounds->x) // Out of the right bounds, new line.
				{
					NextCharacterPosition.x = _posX;
					NextCharacterPosition.y += (32 * _scaleY) + 4;
					if(wordLength > _bounds->x)	//A single word is bigger than the bounds rect
						return rSize;
				}

				if((NextCharacterPosition.y + (rLetter.bottom - rLetter.top) * _scaleY) - _posY > _bounds->y) // Out of the bottom bounds, should not draw.
				{
					return rSize;
				}
			}
///////////////////////////////////////////////////////////////////////////
// BUG FIX END  Reference # BUG-001
//////////////////////////////////////////////////////////////////////////
		}
	}

	return rSize;
}

RECT CBitmapFont::CellAlgorithm(int _cellID)
{
	RECT rCell;

	rCell.left   = (_cellID % m_nNumCols) * m_nCellWidth;
	rCell.top    = (_cellID / m_nNumCols) * m_nCellHeight;
	rCell.right  = rCell.left + m_nCellWidth;
	rCell.bottom = rCell.top  + m_nCellHeight;

	return rCell;
}

D3DXVECTOR2 CBitmapFont::CalculateD3DXVECTOR2(const char* _string, float _scaleX, float _scaleY, D3DXVECTOR2* _bounds)
{
	int id = 0;		// ID of the letter (Char value: A = 65)
	RECT rLetter;	// Draw Rect for the letter

	D3DXVECTOR2 rSize;
	rSize.x = 0;
	rSize.y = 0;

	D3DXVECTOR2 NextCharacterPosition = D3DXVECTOR2(0, 0);

	// Get the sprite for the bitmap
	CSprite* Sprite = SPRITE->GetSprite(m_szFontFileName);
	// If NULL break
	if(!Sprite)
		return rSize;

	// If there is a string
	if(_string)
	{
		//Iterate through it
		for(unsigned int i = 0; i < strlen(_string); ++i)
		{
			if(_string[i] == '\n')
			{
				NextCharacterPosition.x = 0;
				NextCharacterPosition.y += 32 * _scaleY;
			}
			else
			{
				//Get the ID of the letter
				id = static_cast<unsigned char>(_string[i]) - m_cCellStartingChar;

				//Get the Cell
				rLetter = CellAlgorithm(id);

				if(_bounds)
				{
					if((NextCharacterPosition.x + (rLetter.right - rLetter.left) * _scaleX) > _bounds->x) // Out of the right bounds, new line.
					{
						NextCharacterPosition.x = 0;
						NextCharacterPosition.y += 32 * _scaleY;
					}

					if((NextCharacterPosition.y + (rLetter.bottom - rLetter.top) * _scaleY) > _bounds->y) // Out of the bottom bounds, should not draw.
					{
						return rSize;
					}
				}

				// Update the occupied area of the text that is about to draw.
				rSize.x = max(rSize.x, NextCharacterPosition.x + (rLetter.right - rLetter.left) * _scaleX);
				rSize.y = max(rSize.y, NextCharacterPosition.y + (rLetter.bottom - rLetter.top) * _scaleY);

				NextCharacterPosition.x += (rLetter.right - rLetter.left) * _scaleX;
			}
		}
	}

	return rSize;
}

D3DXVECTOR2 CBitmapFont::CalculateD3DXVECTOR2Sprite(const char* _string, float _scaleX, float _scaleY, D3DXVECTOR2* _bounds)
{
	int id = 0;		// ID of the letter (Char value: A = 65)
	RECT rLetter;	// Draw Rect for the letter

	D3DXVECTOR2 rSize;
	rSize.x = 0;
	rSize.y = 0;

	D3DXVECTOR2 NextCharacterPosition = D3DXVECTOR2(0, 0);

	// Get the sprite for the bitmap
	CSprite* Sprite = SPRITE->GetSprite(m_szFontFileName);
	// If NULL break
	if(!Sprite)
		return rSize;

	// If there is a string
	if(_string)
	{
		//Iterate through it
		for(unsigned int i = 0; i < strlen(_string); ++i)
		{
			if(_string[i] == '\n')
			{
				NextCharacterPosition.x = 0;
				NextCharacterPosition.y += 32 * _scaleY;
			}
			else
			{
				//Get the ID of the letter
				id = static_cast<unsigned char>(_string[i]) - m_cCellStartingChar;

				char buffer[8];
				_itoa_s(id, buffer, 8, 10);

				//Get the Cell
				rLetter = Sprite->GetAnimation(0)->GetFrame(buffer)->GetDrawRectangle();

				if(_bounds)
				{
					if((NextCharacterPosition.x + (rLetter.right - rLetter.left) * _scaleX) > _bounds->x) // Out of the right bounds, new line.
					{
						NextCharacterPosition.x = 0;
						NextCharacterPosition.y += 32 * _scaleY;
					}

					if((NextCharacterPosition.y + (rLetter.bottom - rLetter.top) * _scaleY) > _bounds->y) // Out of the bottom bounds, should not draw.
					{
						return rSize;
					}
				}

				// Update the occupied area of the text that is about to draw.
				rSize.x = max(rSize.x, NextCharacterPosition.x + (rLetter.right - rLetter.left) * _scaleX);
				rSize.y = max(rSize.y, NextCharacterPosition.y + (rLetter.bottom - rLetter.top) * _scaleY);

				NextCharacterPosition.x += (rLetter.right - rLetter.left) * _scaleX;
			}
		}
	}

	return rSize;
}