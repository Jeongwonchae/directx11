#include "Stdafx.h"
#include "InputClass.h"



InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass &)
{
}


InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	//���콺 Ŀ���� ��ġ ������ ���� ȭ�� ũ�⸦ ����
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Direct Input �������̽� �ʱ�ȭ
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Ű������ Direct Input �������̽� ����
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//������ �������� ����. �� ��� Ű�����̹Ƿ� ���� ���� �� ������ ������ ��� �� �� ����
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//�ٸ� ���α׷��� �������� �ʵ��� Ű������ ���� ������ ����
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//Ű���带 �Ҵ�
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//���콺 Direct Input �������̽� ����
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//�̸� ���� �� ���콺 ������ ������ ����Ͽ� ���콺�� ������ ������ ����
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	//�ٸ� ���α׷��� ���� �� �� �ֵ��� ���콺�� ���� ������ ����
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//���콺�� �Ҵ�
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputClass::Frame()
{
	if (!ReadKeyboard())
	{
		return false;
	}

	if (!ReadMouse())
	{
		return false;
	}

	//Ű����� ���콺�� ������¸� ó��
	ProcessInput();
	return true;
}

bool InputClass::IsEscapePressed()
{
	//escape Ű�� ���� �������� �ִ��� bit���� ����Ͽ� Ȯ��
	if (m_keyboardState[DIK_ESCAPE]&0x80)
	{
		return true;
	}
	return false;
}

void InputClass::GetMouseLocation(int &mouseX, int &mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

bool InputClass::ReadKeyboard()
{
	//Ű���� ����̽��� ����
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		//Ű���尡 ��Ŀ���� �Ҿ��ų� ȹ����� ���� ��� ��Ʈ���� �ٽ� ������
		if ((result ==DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool InputClass::ReadMouse()
{
	//���콺 ����̽��� ����
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		//���콺�� ��Ŀ���� �Ҿ��ų� ȹ����� ���� ��� ��Ʈ���� �ٽ� ������
		if ((result ==DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void InputClass::ProcessInput()
{
	//������ ���� ���콺 ��ġ�� ������ ������� ���콺 Ŀ���� ��ġ�� ������Ʈ
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	//���콺 ��ġ�� ȭ�� �ʺ� �Ǵ� ���̸� �ʰ����� �ʴ��� Ȯ��
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}