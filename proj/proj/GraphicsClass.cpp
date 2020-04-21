#define _CRT_SECURE_NO_WARNINGS
#include "Stdafx.h"
#include "D3DClass.h"
#include "Cameraclass.h"
#include "BitmapClass.h"
#include "TextureShaderClass.h"
#include "Graphicsclass.h"

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D ��ü ����
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D ��ü �ʱ�ȭ
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera ��ü ����
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// ī�޶� ������ ����
	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);

	// m_Bitmap ��ü ����
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// m_Bitmap ��ü �ʱ�ȭ
	WCHAR* ddsFilename = new WCHAR[sizeof("../proj/data/seafloor.dds")];
	wcscpy(ddsFilename, L"../proj/data/seafloor.dds");
	// m_Bitmap ��ü �ʱ�ȭ
	if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, ddsFilename, 256, 256))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// m_TextureShader ��ü ����
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// m_LightShader ��ü �ʱ�ȭ
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// m_TextureShader ��ü ��ȯ
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// m_Bitmap ��ü ��ȯ
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// m_Camera ��ü ��ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D ��ü ��ȯ
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;

	//�� �������� rotation ������ ������Ʈ
	rotation += (float)XM_PI * 0.004f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	return Render(rotation);
}

bool GraphicsClass::Render(float rotation)
{
	// ���� �׸��� ���� ���۸� ����
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� ����
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� ������
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//��� 2D �������� �����Ϸ��� Z���۸� ��
	m_Direct3D->TurnZBufferOff();

	// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������� �غ�
	m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 100, 100);

	// ���� ���̴��� ����Ͽ� ���� ������
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture()))
	{
		return false;
	}

	//��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� Ŵ
	m_Direct3D->TurnZBufferOn();

	// ������ ������ ȭ�鿡 ����մϴ�
	m_Direct3D->EndScene();

	return true;
}
