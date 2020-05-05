#pragma once

class TextureShaderClass : public AlignedAllocationPolicy<16>
{
private:
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct FontPixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

	struct PixelBufferType
	{
		XMFLOAT4 clipPlane;
	};

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool FontInitialize(ID3D11Device*, HWND);
	bool TextureInitialize(ID3D11Device*, HWND);
	void Shutdown();
	bool FontRender(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	bool TextureRender(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);

private:
	bool FontInitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	bool TextureInitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	bool TextureSetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_constantBuffer = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;
	ID3D11Buffer* m_pixelBuffer = nullptr;
};