#include "Stdafx.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "FpsClass.h"
#include "CpuClass.h"
#include "TimerClass.h"
#include "PositionClass.h"
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
	//������ â ����, ���� ���� ���� �ʱ�ȭ
	int screenWidth = 0;
	int screenHeight = 0;

	//������ ���� �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);

	// m_Input ��ü ���� ����� Ű���� �Է� ó��
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	if (!m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight))
	{
		MessageBox(m_hwnd, L"Could not initialize the input object", L"Error", MB_OK);
		return false;
	}

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	if (!m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd))
	{
		return false;
	}

	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	if (!m_Sound->Initialize(m_hwnd))
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound", L"Error", MB_OK);
		return false;
	}

	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	m_Fps->Initialize();

	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	m_Cpu->Initialize();

	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	if (!m_Timer->Initialize())
	{
		return false;
	}

	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

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
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				break;
			}
		}
		//����ڰ� ESCŰ�� �������� Ȯ�� �� ���� ó����
		if (m_Input->IsEscapePressed() == true)
		{
			break;
		}
	}

}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

bool SystemClass::Frame()
{
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	int mouseX = 0, mouseY = 0;

	//�Է� ������ ó���� ����
	if (!m_Input->Frame())
	{
		return false;
	}

	//�Է� ��ü���� ���콺 ��ġ�� ������
	//m_Input->GetMouseLocation(mouseX, mouseY);

	//������Ʈ �� ��ġ�� ����ϱ� ���� ������ �ð��� ����
	m_Position->SetFrameTime(m_Timer->GetTime());

	//���� �Ǵ� ������ ȭ��ǥ Ű�� �������� Ȯ��, �׷��� ������ ī�޶� ������ ȸ��
	bool keyDown = m_Input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);

	//���� �� ����Ʈ ȸ���� ������
	float rotationY = 0.0f;
	m_Position->GetRotation(rotationY);

	if (!m_Graphics->Frame(m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime(), rotationY))
	{
		return false;
	}

	return m_Graphics->Render();
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
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

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