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
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라 포지션 설정
	XMMATRIX baseViewMatrix;
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// m_Text 객체 생성
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// m_Text 객체 초기화
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

	// m_Text 객체 반환
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// m_Camera 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
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

	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	m_LightMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureArray());

	//절두체를 만듬
	//m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	//렌더링 될 모델의 수를 얻음
	//int modelCount = m_ModelList->GetModelCount();

	//렌더링 된 모델의 개수를 초기화
	//int renderCount = 0;

	//모든 모델을 살펴보고 카메라 뷰에서 볼 수 있는 경우에만 렝더링 함
	//for (int index = 0; index < modelCount; index++)
	//{
		//해당 인덱스에서 구형 모델의 위치와 색상을 가져옴
		//m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		//구형 모델이 뷰 frustum에 있는지 확인
		//if (m_Frustum->CheckSphere(positionX, positionY, positionZ, radius))
		//{
			//모델을 렝더링 할 위치로 이동
			//worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

			//모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 드로잉을 준비)
			//m_Model->Render(m_Direct3D->GetDeviceContext());

			//라이트 셰이더를 사용하여 모델을 렌더링함
			//m_LightMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureArray(), m_Light->GetDirection(), color);

			//원래의 월드 매트릭스로 리셋
			//m_Direct3D->GetWorldMatrix(worldMatrix);

			//renderCount++;
		//}
	//}

	//이 프레임에서 실제로 렌더링 된 모델의 수를 설정
	//if (!m_Text->SetRenderCount(renderCount, m_Direct3D->GetDeviceContext()))
	//{
	//	return false;
	//}

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
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

	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_Direct3D->TurnZBufferOn();

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}