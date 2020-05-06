#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
class CameraClass;
class ModelClass;
class ShaderClass;
class TextClass;
class LightClass;
class RenderTextureClass;
class DebugWindowClass;
class TextureShaderClass;
class TransparentShaderClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float, float);
	bool Render();

private:
	bool RenderToTexture();
	bool RenderScene();

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;
	ModelClass* m_Model2 = nullptr;
	ShaderClass* m_Shader = nullptr;
	TextClass* m_Text = nullptr;
	LightClass* m_Light = nullptr;
	RenderTextureClass* m_RenderTexture = nullptr;
	DebugWindowClass* m_DebugWindow = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	TransparentShaderClass* m_TransparentShader = nullptr;

};

