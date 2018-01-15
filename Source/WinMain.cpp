#include <windows.h>

//#define VLD_AGGREGATE_DUPLICATES//Dont report the same leak 50 times!
//#define VLD_MAX_DATA_DUMP 0 //Dont show me random hex values!
#include <vld.h>

#include "../resource.h" // For Icon ID
#include <process.h>
#include <commdlg.h>
#include "CGame.h"
#include "Managers/Bad Chef/Networking/CNetworkManager.h"

namespace
{
	CGame* Game;
};

LRESULT CALLBACK MessageHandler(HWND Window, UINT Message, WPARAM wParameter, LPARAM lParameter);

unsigned int WINAPI NetworkReceive(void* lpvArgList);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Initialize window strings.
	char WindowClassName[] = "FoodFightWindowClass";
	char WindowCaption[] = "Food Fight";

	// Parse command line arguments.
	const bool Windowed = true;
	RECT WindowArea;
	WindowArea.left = 0;
	WindowArea.top = 0;
	WindowArea.right = 1024;
	WindowArea.bottom = 768;

	// Create the window class.
	WNDCLASSEX WindowClass;
	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = MessageHandler;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = hInstance;
	WindowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	WindowClass.hIconSm = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL; 
	WindowClass.lpszClassName = WindowClassName;

	// Register window class.
	RegisterClassEx(&WindowClass);

	// Setup window style flags.
	DWORD WindowStyles = 0;
	WindowStyles = WS_VISIBLE;
	WindowStyles |= Windowed ? WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU : WS_POPUP;

	DWORD ExWindowStyles = 0;
	ExWindowStyles = WS_EX_APPWINDOW;

	// Window creation.
	HWND Window = CreateWindowEx(ExWindowStyles, WindowClass.lpszClassName, WindowCaption, WindowStyles, WindowArea.left, WindowArea.top, (WindowArea.right - WindowArea.left), (WindowArea.bottom - WindowArea.top), NULL, NULL, hInstance, NULL);

	// Display the window.
	ShowWindow(Window, nShowCmd);
	UpdateWindow(Window);

	// Game creation.
	Game = CGame::GetInstance();

	// Game initialization.
	CGame::SGameParameters GameParameters;
	GameParameters.ApplicationInstance = hInstance;
	GameParameters.Window = Window;
	GameParameters.Windowed = Windowed;
	GameParameters.VSync = false;
	GameParameters.ClientWidth = (WindowArea.right - WindowArea.left);
	GameParameters.ClientHeight = (WindowArea.bottom - WindowArea.top);

	if(!Game->Initialize(GameParameters))
		return false;

	//Initialize networking
	CRITICAL_SECTION networkCS;
	InitializeCriticalSection(&networkCS);
	CNetworkManager::SetCS(&networkCS);
	CNetworkManager::GetInstance();
	// Create the Network Thread:
	HANDLE hNetworkThread = (HANDLE)_beginthreadex(NULL, 0, NetworkReceive, (void*)NULL, 0, NULL);

	// Running loop.
	MSG Message;
	do
	{
		if(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		if(!Game->Run())
			PostQuitMessage(0);

	} while(Message.message != WM_QUIT);

	// Game destruction.
	Game->DeleteInstance();

	//Shutdown the network manager
	CNetworkManager::Lock();
	CNetworkManager::GetInstance()->Shutdown();
	CNetworkManager::Unlock();

	// Stop Networking Thread:
	WaitForSingleObject(hNetworkThread, INFINITE);
	// Destroy the Networking Thread:
	CloseHandle(hNetworkThread);

	//Network Manager destruction.
	CNetworkManager::DeleteInstance();
	DeleteCriticalSection(&networkCS);

	return Message.wParam;
}

LRESULT CALLBACK MessageHandler(HWND Window, UINT Message, WPARAM wParameter, LPARAM lParameter)
{
	switch(Message)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATE:
		{
			//	gaining focus
			if (LOWORD(wParameter) != WA_INACTIVE)
			{
				// unpause game code here
				//CSGD_DirectInput::GetInstance()->AcquireAll();
			}
			else // losing focus
			{
				// pause game code here
#ifndef _DEBUG
				Game->m_FocusLost.Trigger(Game);
#endif
			}
			break;
		}
		default:
			break;
	}
	return DefWindowProc(Window, Message, wParameter, lParameter);
}

// The Network Thread:
unsigned int WINAPI NetworkReceive(void* lpvArgList)
{
	while(true)
	{
		if(CNetworkManager::GetInstance()->CheckRunning())
		{
			CNetworkManager::GetInstance()->Receive();
			Sleep(1);
		}
		else
			break;
	}

	return 0;
}

#if _DEBUG
//The Main function
//Used to create a console window
int main(void)
{
	LOG("The return of console!");
//#define LEAK_LINE 100
//	_CrtSetBreakAlloc(LEAK_LINE);
//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWDEFAULT);
	return 0;
}

#endif