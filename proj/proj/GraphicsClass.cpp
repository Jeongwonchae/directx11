#define _CRT_SECURE_NO_WARNINGS
#include "Stdafx.h"
#include "D3DClass.h"
#include "Cameraclass.h"
#include "Modelclass.h"
#include "LightClass.h"
#include "LightShaderClass.h"
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

	// m_Model ��ü ����
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// m_Model ��ü �ʱ�ȭ
	WCHAR* ddsFilename = new WCHAR[sizeof("../proj/data/seafloor.dds")];
	char* modelFilename = new char[sizeof("../proj/data/cube.txt")];
	wcscpy(ddsFilename, L"../proj/data/seafloor.dds");
	strcpy(modelFilename, "../proj/data/cube.txt");
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), modelFilename, ddsFilename))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// m_LightShader ��ü ����
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// m_LightShader ��ü �ʱ�ȭ
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
		return false;
	}

	// m_Light ��ü ����
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	//m_Light ��ü �ʱ�ȭ
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}

void GraphicsClass::Shutdown()
{
	// m_Light ��ü ��ȯ
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// m_LightShader ��ü ��ȯ
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// m_Model ��ü ��ȯ
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//�ﰢ���� ȸ�� �� �� �ֵ��� ȸ�� ������ ���� ����� ȸ��
	worldMatrix = XMMatrixRotationY(rotation);

	// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������� �غ�
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// ���� ���̴��� ����Ͽ� ���� ������
	if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
	{
		return false;
	}

	// ������ ������ ȭ�鿡 ����մϴ�
	m_Direct3D->EndScene();

	return true;
}
