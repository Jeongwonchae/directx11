#include "Stdafx.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SystemClass.h"

SystemClass::SystemClass()
{
}

SystemClass::SystemClass(const SystemClass &)
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	//윈도우 창 가로, 세로 넓이 변수 초기화
	int screenWindth = 0;
	int screenHeight = 0;

	//윈도우 생성 초기화
	InitializeWindows(screenWindth, screenHeight);

	// m_Input 객체 생성 사용자 키보드 입력 처리
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	m_Input->Initialize();

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	return m_Graphics->Initialize(screenWindth, screenHeight, m_hwnd);
}

void SystemClass::Shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!Frame())
				break;
		}
	}
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}
	case WM_KEYUP:
	{
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

bool SystemClass::Frame()
{
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	return m_Graphics->Frame();
}

void SystemClass::InitializeWindows(int &screenWidth, int &screenHeight)
{
	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"Dx11";

	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = 0;
	int posY = 0;

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}

void SystemClass::ShutdownWindows()
{
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}