/**
 * 0===========================================================================0
 * | CCamera                                                                   |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Author        : Joseph Stuhr                                            |
 * |   Version       : 01.00 J.Stuhr                                           |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   File Name     : CCamera.cpp                                             |
 * |   Bundle        : FF_JS_CCAMERA_H_                                        |
 * |   Line Count    : 000,295                                                 |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Date Created  : 01:15 pm (Eastern) on 09/16/2009                        |
 * |   Date Edited   : 03:32 pm (Eastern) on 09/16/2009                        |
 * 0=====0===============================================================0=====0
 *       |                                                               |      
 * 0=====0===============================================================0=====0
 * | Code Description:                                                         |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   A class used to store the camera information as well as move the camera |
 * |    withing the game world.                                                |
 * |                                                                           |
 * |   Objects Position = Position - CameraPosition                            |
 * 0===========================================================================0
 * | Version History Key:                                                      |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   [H] - Upgraded Comment Header.                                          |
 * |                                                                           |
 * |   [$] - Project name change.         [~] - Author change.                 |
 * |   [#] - Bundle name change.          [.] - File name change.              |
 * |   [;] - Updated Description.         [&] - Updated Files Needed.          |
 * |   [=] - Addition to credits.                                              |
 * |                                                                           |
 * |   [+] - Addition to the code.        [-] - Subtraction from code.         |
 * |   [^] - Improvement to the code.     [%] - Optimization to the code.      |
 * |   [!] - Known bug.                   [*] - Bug fix.                       |
 * |   [@] - Planned change/feature.                                           |
 * |                                                                           |
 * |   [c] - Added comments.              [x] - Removed comments.              |
 * |   [v] - Edited comments.                                                  |
 * |                                                                           |
 * |   [ ] - Other                                                             |
 * |                                                                           |
 * |   [a] - Alpha release.               [b] - Beta release.                  |
 * |   [i] - Initial public release.      [f] - Final release from (author).   |
 * |                                                                           |
 * 0===========================================================================0
 * | Version History:                                                          |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Version 01.00 J.Stuhr - 09/16/2009                                      |
 * |    [i] - Initial release from Joseph Stuhr (J.Stuhr).                     |
 * 0===========================================================================0
 * | Files Needed:                                                             |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   NONE                                                                    |
 * 0===========================================================================0
 * | How to use the File:                                                      |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Call "InitCamera();" without quotes to create your camera. The  |
 * |    camera needs some details so it will work correctly that will be passed|
 * |    in as parameters to InitCamera().                                      |
 * |      _clientWidth  : Width of the Camera                                  |
 * |	  _clientHeight : Height of the Camera                                 |
 * |	  _mapWidht     : Width of the Map or Play Area                        |
 * |	  _mapHeight    : Height of the Map or Play Area                       |
 * |	  _startingPosX : *OPTIONAL* - X Position of the Camera                |
 * |	  _startingPosY : *OPTIONAL* - Y Position of the Camera                |
 * |	  _cameraSpeed  : *OPTIONAL* - Speed of the Camera                     |
 * |                                                                           |
 * |   Call "ShutdownCamera();" to shut down the camera.               |
 * 0=====0===============================================================0=====0
 *       |                                                               |      
 * 0=====0===============================================================0=====0
 * | Comment Header SDK : Version 02.00 J.Stuhr - 12/03/2008                   |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   This header is free software: you can redistribute it and/or modify     |
 * |     it under the terms of the GNU General Public License as published by  |
 * |     the Free Software Foundation, either version 3 of the License, or     |
 * |     any later version, as long as this section remains intact.            |
 * |                                                                           |
 * |   This header is distributed in the hope that it will be useful,          |
 * |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
 * |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
 * |     GNU General Public License for more details.                          |
 * |                                                                           |
 * |   You should have received a copy of the GNU General Public License       |
 * |     along with this program.  If not, see <http://www.gnu.org/licenses/>. |
 * |                                                                           |
 * |   Contact: JoeStuhr@Gmail.com       Copyright (c) 2008-2010 Joseph Stuhr. |
 * 0===========================================================================0
 * |      111111111122222222223333333333444444444455555555556666666666777777777|
 * |456789012345678901234567890123456789012345678901234567890123456789012345678|
 * |                                                            ~ 80 Columns ~ |
 * 0===========================================================================0
 * | Credits:                                                                  |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   CCamera - Version 01.00 J.Stuhr:                                        |
 * |     Joseph Stuhr ~ Initial coder.                                         |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Comment Header - Version 02.00 J.Stuhr:                                 |
 * |     Joseph Stuhr ~ Initial author.                                        |
 * 0===========================================================================0
 */

#include "CCamera.h"
#include "../GUI Objects/CCursor.h"

namespace
{
	const unsigned char CURSOR_SCROLL_OFFSET = 20;
}

// Initialize the static object
CCamera* CCamera::m_Instance = NULL;

// 0===========================================================================0
// | Function  :	CCamera()                                         09/16/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	void                                                       |
// |                                                                           |
// |   Return  :	N/A                                                        |
// |                                                                           |
// |   Purpose :	Constructor.                                               |
// 0===========================================================================0
CCamera::CCamera(void)
{
	m_Position = D3DXVECTOR2(0, 0);

	m_fClientWidth    = 1024.0f;
	m_fClientHeight   = 768.0f;

	m_fMapWidth       = 1024.0f;
	m_fMapHeight      = 768.0f;
}

// 0===========================================================================0
// | Function  :	~CCamera()                                        09/16/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	void                                                       |
// |                                                                           |
// |   Return  :	N/A                                                        |
// |                                                                           |
// |   Purpose :	Destructor.                                                |
// 0===========================================================================0
CCamera::~CCamera(void)
{
	m_Instance = NULL;
}

CCamera* CCamera::GetInstance()
{
	if(!m_Instance)
		m_Instance = new CCamera;
	return m_Instance;
}

void CCamera::DeleteInstance()
{
	if(m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

// 0===========================================================================0
// | Function  :	InitCamera()                                      09/16/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	_clientWidth  : Width of the client.                       |
// |                _clientHeight : Height of the client.                      |
// |                _mapWidht     : Width of the current map.                  |
// |                _mapHeight    : Height of the current map.                 |
// |                _startingPosX : X Position of the Camera.                  |
// |                _startingPosY : Y Position of the Camera.                  |
// |                _cameraSpeed  : Speed the camera moves at.                 |
// |                                                                           |
// |   Return  :	True if the function was successful.                       |
// |                                                                           |
// |   Purpose :	To initialize the Camera.                                  |
// 0===========================================================================0
bool CCamera::InitCamera(float _clientWidth,  float _clientHeight, float _mapWidth,   float _mapHeight,
						 float _startingPosX, float _startingPosY, float _cameraSpeed                  )
{
	SetClientWidth(_clientWidth); SetClientHeight(_clientHeight);
	SetMapWidth(_mapWidth); SetMapHeight(_mapHeight);
	SetPosition(_startingPosX, _startingPosY);
	SetVelocity(0, 0);
	SetCameraSpeed(_cameraSpeed);

	return true;
}

void CCamera::Update(float _delta)
{
	CCursor* Cursor = CCursor::GetInstance();

	Translate(GetVelocity() * _delta);

	D3DXVECTOR2 PreviousPosition(GetPosition());
	D3DXVECTOR2 Translation(0, 0);

	if(Cursor->GetPositionX() <= GetPositionX())
			Translation.x = -GetCameraSpeed() * _delta;
	if(Cursor->GetPositionX() >= (GetClientWidth() + GetPositionX()))
			Translation.x = GetCameraSpeed() * _delta;

	if(Cursor->GetPositionY() <= GetPositionY())
			Translation.y = -GetCameraSpeed() * _delta;
	if(Cursor->GetPositionY() >= (GetClientHeight() + GetPositionY()))
			Translation.y = GetCameraSpeed() * _delta;
	
	if(D3DXVec2LengthSq(&Translation))
		Translate(Translation);

	SetVelocity(0, 0);
}

D3DXVECTOR2 CCamera::GetPosition() const
{
	return m_Position;
}

float CCamera::GetPositionX() const
{
	return m_Position.x;
}

float CCamera::GetPositionY() const
{
	return m_Position.y;
}

void CCamera::SetPosition(D3DXVECTOR2 Position)
{
	D3DXVECTOR2 PreviousPosition(GetPosition());
	SetPositionX(Position.x);
	SetPositionY(Position.y);
}

void CCamera::SetPosition(float X, float Y)
{
	SetPositionX(X);
	SetPositionY(Y);
}

void CCamera::SetPositionX(float X)
{
	float PreviousPosition = GetPositionX();
	m_Position.x = max(0, min(X, GetMapWidth()-GetClientWidth()));
	CCursor::GetInstance()->TranslateX(GetPositionX() - PreviousPosition);
}

void CCamera::SetPositionY(float Y)
{
	float PreviousPosition = GetPositionY();
	m_Position.y = max(0, min(Y, GetMapHeight()-GetClientHeight()));
	CCursor::GetInstance()->TranslateY(GetPositionY() - PreviousPosition);
}

void CCamera::Translate(D3DXVECTOR2 Distance)
{
	SetPosition(GetPosition() + Distance);
}

void CCamera::Translate(float X, float Y)
{
	TranslateX(X);
	TranslateY(Y);
}

void CCamera::TranslateX(float X)
{
	SetPositionX(GetPositionX() + X);
}

void CCamera::TranslateY(float Y)
{
	SetPositionY(GetPositionY() + Y);
}

D3DXVECTOR2 CCamera::GetVelocity() const
{
	return m_Velocity;
}

float CCamera::GetVelocityX() const
{
	return m_Velocity.x;
}

float CCamera::GetVelocityY() const
{
	return m_Velocity.y;
}

void CCamera::SetVelocity(D3DXVECTOR2 Velocity)
{
	m_Velocity = Velocity;
}

void CCamera::SetVelocity(float X, float Y)
{
	SetVelocityX(X);
	SetVelocityY(Y);
}

void CCamera::SetVelocityX(float X)
{
	m_Velocity.x = X;
}

void CCamera::SetVelocityY(float Y)
{
	m_Velocity.y = Y;
}

void CCamera::Accelerate(D3DXVECTOR2 Velocity)
{
	SetVelocity(GetVelocity() + Velocity);
}

void CCamera::Accelerate(float X, float Y)
{
	AccelerateX(X);
	AccelerateY(Y);
}

void CCamera::AccelerateX(float X)
{
	SetVelocityX(GetVelocityX() + X);
}

void CCamera::AccelerateY(float Y)
{
	SetVelocityY(GetVelocityY() + Y);
}

RECT CCamera::GetViewRect(void)
{
	RECT rTemp;
	rTemp.top = (LONG)GetPositionY();
	rTemp.left = (LONG)GetPositionX();
	rTemp.right = rTemp.left + (LONG)GetClientWidth();
	rTemp.bottom = rTemp.top + (LONG)GetClientHeight();

	return rTemp;
}