#include "Stdafx.h"
#include "RenderTextureClass.h"

RenderTextureClass::RenderTextureClass()
{
}

RenderTextureClass::RenderTextureClass(const RenderTextureClass &)
{
}


RenderTextureClass::~RenderTextureClass()
{
}

bool RenderTextureClass::Initialize(ID3D11Device *device, int textureWidth, int textureHeight)
{
	return false;
}

void RenderTextureClass::Shutdown()
{
}

void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext *deviceContext, ID3D11DepthStencilView *depthStencilView)
{
}

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext *deviceContext, ID3D10DepthStencilView *depthStencilView, 
	float red, float green, float blue, float alpha)
{
}

ID3D11ShaderResourceView * RenderTextureClass::GetShaderResourceView()
{
	return nullptr;
}
