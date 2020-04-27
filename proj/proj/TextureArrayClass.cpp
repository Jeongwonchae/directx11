#include "Stdafx.h"
#include "TextureArrayClass.h"


TextureArrayClass::TextureArrayClass()
{
}


TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{
}


TextureArrayClass::~TextureArrayClass()
{
}

bool TextureArrayClass::Initialize(ID3D11Device *device, WCHAR *filename)
{
	if (FAILED(CreateDDSTextureFromFile(device, filename, nullptr, &m_TextureArrays[0])))
	{
		return false;
	}
	return true;
}

bool TextureArrayClass::Initialize(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3)
{
	// 텍스처를 파일로부터 읽어온다
	if (FAILED(CreateDDSTextureFromFile(device, filename1, nullptr, &m_TextureArrays[0])))
	{
		return false;
	}
	if (FAILED(CreateDDSTextureFromFile(device, filename2, nullptr, &m_TextureArrays[1])))
	{
		return false;
	}

	if (FAILED(CreateDDSTextureFromFile(device, filename3, nullptr, &m_TextureArrays[2])))
	{
		return false;
	}

	return true;
}




void TextureArrayClass::Shutdown()
{
	//텍스처 뷰 리소스를 해제한다.
	if (m_TextureArrays[0])
	{
		m_TextureArrays[0]->Release();
		m_TextureArrays[0] = 0;
	}
	if (m_TextureArrays[1])
	{
		m_TextureArrays[1]->Release();
		m_TextureArrays[1] = 0;
	}
	if (m_TextureArrays[2])
	{					
		m_TextureArrays[2]->Release();
		m_TextureArrays[2] = 0;
	}
}


ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_TextureArrays;
}

ID3D11ShaderResourceView* TextureArrayClass::GetTexture()
{
	return m_TextureArrays[0];
}
