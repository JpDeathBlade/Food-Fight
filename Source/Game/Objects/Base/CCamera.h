/**
* 0===========================================================================0
* | CCamera                                                                   |
* |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
* |   Author        : Joseph Stuhr                                            |
* |   Version       : 01.00 J.Stuhr                                           |
* |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
* |   File Name     : CCamera.h                                               |
* |   Bundle        : FF_JS_CCAMERA_H_                                        |
* |   Line Count    : 000,266                                                 |
* |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
* |   Date Created  : 01:15 pm (Eastern) on 09/16/2009                        |
* |   Date Edited   : 03:26 pm (Eastern) on 09/16/2009                        |
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
* |   Call "CAMERA->InitCamera();" without quotes to create your camera. The  |
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
* |   Call "CAMERA->ShutdownCamera();" to shut down the camera.               |
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

#ifndef FF_JS_CCAMERA_H_
#define FF_JS_CCAMERA_H_

#include <windows.h>
#include "../../../Managers/Bad Chef/Events/CEvent.h"
#include <d3dx9.h>

// Camera MACRO to return the Camera Instance
#define CAMERA		 CCamera::GetInstance()

class CCamera
{
	static CCamera* m_Instance;		// Instance of the CCamera

	D3DXVECTOR2 m_Position;
	D3DXVECTOR2 m_Velocity;

	float m_fCameraSpeed;			// The speed of the camera

	float m_fClientWidth;			// Width of the client
	float m_fClientHeight;			// Height of the client

	float m_fMapWidth;				// Width of the current map
	float m_fMapHeight;				// Height of the current map

	CCamera();
	CCamera(const CCamera& Source);
	CCamera& operator=(const CCamera& Source);
public:
	~CCamera();

	// 0===================================================================0
	// | Function  :	GetInstance()                             09/16/09 |
	// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
	// |   Input   :	void                                               |
	// |                                                                   |
	// |   Return  :	Instance to this class.                            |
	// |                                                                   |
	// |   Purpose :	Make the class a singleton and allow access to it. |
	// 0===================================================================0
	static CCamera* GetInstance();

	static void DeleteInstance();

	// 0===================================================================0
	// | Function  :	InitCamera()                              09/16/09 |
	// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
	// |   Input   :	_clientWidth  : Width of the client.               |
	// |                _clientHeight : Height of the client.              |
	// |                _mapWidht     : Width of the current map.          |
	// |                _mapHeight    : Height of the current map.         |
	// |                _startingPosX : X Position of the Camera.          |
	// |                _startingPosY : Y Position of the Camera.          |
	// |                _cameraSpeed  : Speed the camera moves at.         |
	// |                                                                   |
	// |   Return  :	True if the function was successful.               |
	// |                                                                   |
	// |   Purpose :	To initialize the Camera.                          |
	// 0===================================================================0
	bool InitCamera(float _clientWidth, float _clientHeight, float _mapWidth, float _mapHeight,
		float _startingPosX = 0.0f, float _startingPosY = 0.0f, float _cameraSpeed = 3.0f);

	void Update(float _delta);

	D3DXVECTOR2 GetPosition() const; // Returns the object's position.
	float GetPositionX() const; // Returns the object's position's x-coordinate.
	float GetPositionY() const; // Returns the object's position's y-coordinate.

	void SetPosition(D3DXVECTOR2 Position); // Sets the object's position.
	void SetPosition(float X, float Y); // Sets the object's position.
	void SetPositionX(float X); // Sets the object's position's x-coordinate.
	void SetPositionY(float Y); // Sets the object's position's y-coordinate.

	void Translate(D3DXVECTOR2 Distance); // Translates the object.
	void Translate(float X, float Y); // Translates the object.
	void TranslateX(float X); // Translates the object in the x-axis.
	void TranslateY(float Y); // Translates the object in the y-axis.

	D3DXVECTOR2 GetVelocity() const; // Returns the object's velocity.
	float GetVelocityX() const; // Returns the object's velocity's x-coordinate.
	float GetVelocityY() const; // Returns the object's velocity's y-coordinate.

	void SetVelocity(D3DXVECTOR2 Velocity); // Sets the object's velocity.
	void SetVelocity(float X, float Y); // Sets the object's velocity.
	void SetVelocityX(float X); // Sets the object's velocity's x-coordinate.
	void SetVelocityY(float Y); // Sets the object's velocity's y-coordinate.

	void Accelerate(D3DXVECTOR2 Velocity); // Accelerates the object.
	void Accelerate(float X, float Y); // Accelerates the object.
	void AccelerateX(float X); // Accelerates the object in the x-axis.
	void AccelerateY(float Y); // Accelerates the object in the y-axis.

	// 0===================================================================0
	// | ~ ACCESSORS ~                                                     |
	// 0===================================================================0
	float GetClientWidth	(void)		{ return m_fClientWidth;     }
	float GetClientHeight	(void)		{ return m_fClientHeight;	 }
	float GetMapWidth		(void)		{ return m_fMapWidth;		 }
	float GetMapHeight		(void)		{ return m_fMapHeight;		 }
	float GetCameraSpeed	(void)		{ return m_fCameraSpeed;     }
	RECT  GetViewRect		(void);

	// 0===================================================================0
	// | ~ MODIFIERS ~                                                     |
	// 0===================================================================0
	void SetClientWidth(float _width)	{ m_fClientWidth  = _width;		}
	void SetClientHeight(float _height)	{ m_fClientHeight = _height;	}
	void SetMapWidth(float _width)		{ m_fMapWidth     = _width;		}
	void SetMapHeight(float _height)	{ m_fMapHeight    = _height;	}
	void SetCameraSpeed(float _speed)	{ m_fCameraSpeed  = _speed;		}
}; // end of class CCamera

#endif // end of FF_JS_CCAMERA_H_