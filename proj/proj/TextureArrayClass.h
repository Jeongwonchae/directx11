#pragma once

class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	bool Initialize(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();
	ID3D11ShaderResourceView* GetTexture();


private:
	ID3D11ShaderResourceView* m_TextureArrays[3] = { nullptr ,nullptr, nullptr};
};
