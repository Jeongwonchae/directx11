#include "Stdafx.h"
#include "TextureClass.h"
#include "FontClass.h"

#include <fstream>
using namespace std;

FontClass::FontClass()
{
}

FontClass::FontClass(const FontClass &)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device *device, char *fontFilename, WCHAR *textureFilename)
{
	//폰트 파일 로드
	if (!LoadFontData(fontFilename))
	{
		return false;
	}

	//텍스처를 로드하고 폰트를 가져온다
	return LoadTexture(device, textureFilename);
}

void FontClass::Shutdown()
{
	//폰트 텍스처 반환
	ReleaseTexture();

	//폰트 데이터 반환
	ReleaseFontData();
}

ID3D11ShaderResourceView * FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void *vertices, char *sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	//vertexPtr에 vertices를 VertexType으로 강제 변환하여 저장
	vertexPtr = (VertexType*)vertices;

	//문장의 길이를 저장
	numLetters = (int)strlen(sentence);

	index = 0;

	//화면에 각 문장들을 그림
	for ( i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		//만약 문자가 공백이면 픽셀을 이동시킴
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			//첫번째
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY -16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			//두번째
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, (drawY - 16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			//drawX 위치를 글자크기보다 조금 크게 변경
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}

bool FontClass::LoadFontData(char * filename)
{
	ifstream fin;
	int i;
	char temp;

	//m_Font 생성
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	//폰트 사이즈와 문자사이의 간격을 읽어냄
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	//95개의 아스키코드 문자들을 읽음
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	//파일을 닫음
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	//폰트 데이터 배열 반환
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}
}

bool FontClass::LoadTexture(ID3D11Device *device , WCHAR *filename)
{
	//m_Texture 생성
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//Texture객체 초기화
	return m_Texture->Initialize(device, filename);
}

void FontClass::ReleaseTexture()
{
	//텍스처 객체 반환
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}
