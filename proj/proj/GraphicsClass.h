#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
class CameraClass;
class ModelClass;
class BumpMapShaderClass;
class TextClass;
class LightClass;
//class ModelListClass;
//class FrustumClass;

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
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;
	BumpMapShaderClass* m_BumpMapShader = nullptr;
	TextClass* m_Text = nullptr;
	LightClass* m_Light;
	//ModelListClass* m_ModelList = nullptr;
	//FrustumClass* m_Frustum = nullptr;
};

