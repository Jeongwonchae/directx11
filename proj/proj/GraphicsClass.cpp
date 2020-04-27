#include "Stdafx.h"
#include "D3dclass.h"
#include "Cameraclass.h"
#include "ModelClass.h"
#include "LightMapShaderClass.h"
#include "Textclass.h"
//#include "LightClass.h"
//#include "ModelListClass.h"
//#include "FrustumClass.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
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
	XMMATRIX baseViewMatrix;
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// m_Text ��ü ����
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// m_Text ��ü �ʱ�ȭ
	if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix))
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}


	m_Model = new ModelClass;
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), (char*)"../proj/data/square.txt", (WCHAR*)L"../proj/data/stone01.dds", (WCHAR*)L"../proj/data/light01.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_LightMapShader = new LightMapShaderClass;
	if (!m_LightMapShader)
	{
		return false;
	}

	if (!m_LightMapShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Lightmap shader object.", L"Error", MB_OK);
		return false;
	}

	//m_Light = new LightClass;
	//if (!m_Light)
	//{
	//	return false;
	//}

	//m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	//m_ModelList = new ModelListClass;
	//if (!m_ModelList)
	//{
	//	return false;
	//}

	//if (!m_ModelList->Initialize(25))
	//{
	//	MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
	//	return false;
	//}

	//m_Frustum = new FrustumClass;
	//if (!m_Frustum)
	//{
	//	return false;
	//}

	return true;
}


void GraphicsClass::Shutdown()
{
	//if (m_Frustum)
	//{
	//	delete m_Frustum;
	//	m_Frustum = 0;
	//}

	//if (m_ModelList)
	//{
	//	m_ModelList->Shutdown();
	//	delete m_ModelList;
	//	m_ModelList = 0;
	//}

	//if (m_Light)
	//{
	//	delete m_Light;
	//	m_Light = 0;
	//}

	if (m_LightMapShader)
	{
		m_LightMapShader->Shutdown();
		delete m_LightMapShader;
		m_LightMapShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// m_Text ��ü ��ȯ
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
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


bool GraphicsClass::Frame(int fps, int cpu, float frameTime, float rotationY)
{
	m_Camera->SetPosition(0.0f, 0.0f, -3.0f);

	//m_Camera->SetRotation(0.0f, rotationY, 0.0f);

	if (!m_Text->SetFps(fps, m_Direct3D->GetDeviceContext()))
	{
		return false;
	}

	if (!m_Text->SetCpu(cpu, m_Direct3D->GetDeviceContext()))
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	//float positionX = 0;
	//float positionY = 0;
	//float positionZ = 0;
	//float radius = 1.0f;
	//XMFLOAT4 color;

	// ���� �׸��� ���� ���۸� ����ϴ�
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	m_LightMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureArray());

	//����ü�� ����
	//m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	//������ �� ���� ���� ����
	//int modelCount = m_ModelList->GetModelCount();

	//������ �� ���� ������ �ʱ�ȭ
	//int renderCount = 0;

	//��� ���� ���캸�� ī�޶� �信�� �� �� �ִ� ��쿡�� ������ ��
	//for (int index = 0; index < modelCount; index++)
	//{
		//�ش� �ε������� ���� ���� ��ġ�� ������ ������
		//m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		//���� ���� �� frustum�� �ִ��� Ȯ��
		//if (m_Frustum->CheckSphere(positionX, positionY, positionZ, radius))
		//{
			//���� ������ �� ��ġ�� �̵�
			//worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

			//�� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������� �غ�)
			//m_Model->Render(m_Direct3D->GetDeviceContext());

			//����Ʈ ���̴��� ����Ͽ� ���� ��������
			//m_LightMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureArray(), m_Light->GetDirection(), color);

			//������ ���� ��Ʈ������ ����
			//m_Direct3D->GetWorldMatrix(worldMatrix);

			//renderCount++;
		//}
	//}

	//�� �����ӿ��� ������ ������ �� ���� ���� ����
	//if (!m_Text->SetRenderCount(renderCount, m_Direct3D->GetDeviceContext()))
	//{
	//	return false;
	//}

	// ��� 2D �������� �����Ϸ��� Z ���۸� ���ϴ�.
	m_Direct3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text strings.
	if (!m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix))
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
	m_Direct3D->TurnZBufferOn();

	// ������ ������ ȭ�鿡 ����մϴ�
	m_Direct3D->EndScene();

	return true;
}