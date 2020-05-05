#include "Stdafx.h"
#include "D3dclass.h"
#include "Cameraclass.h"
#include "ModelClass.h"
#include "ShaderClass.h"
#include "Textclass.h"
#include "LightClass.h"
#include "RenderTextureClass.h"
#include "DebugWindowClass.h"
#include "TextureShaderClass.h"
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
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), (char*)"../proj/data/cube.txt", (WCHAR*)L"../proj/data/stone02.dds",
		(WCHAR*)L"../proj/data/bump02.dds", (WCHAR*)L"../proj/data/spec02.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Shader = new ShaderClass;
	if (!m_Shader)
	{
		return false;
	}

	if (!m_Shader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	if (!m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight))
	{
		return false;
	}

	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	if (!m_DebugWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, 100, 100) )
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	if (!m_TextureShader->TextureInitialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

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

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_Shader)
	{
		m_Shader->Shutdown();
		delete m_Shader;
		m_Shader = 0;
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
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

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

	//전체 장면을 텍스처로 렌더링
	if (!RenderToTexture())
	{
		return false;
	}

	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(0.0f, 0.0f, 1.0f, 1.0f);

	if (!RenderScene())
	{
		return false;
	}

	m_Direct3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//if (!m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), 100, 100))
	//{
	//	return false;
	//}

	//if (!m_TextureShader->TextureRender(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView()))
	//{
	//	return false;
	//}

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

bool GraphicsClass::RenderToTexture()
{
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	if (!RenderScene())
	{
		return false;
	}

	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene()
{
	XMFLOAT4 clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);


	//각 프레임의 rotation 변수를 업데이트
	static float rotation = 0.0f;
	rotation += (float)XM_PI * 0.0025f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	worldMatrix = XMMatrixRotationY(rotation);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	return m_TextureShader->TextureRender(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), clipPlane);

	/*return m_Shader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureArray(),
			m_Light->GetDirection(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());*/
}
