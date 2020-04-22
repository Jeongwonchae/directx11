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
	//��Ʈ ���� �ε�
	if (!LoadFontData(fontFilename))
	{
		return false;
	}

	//�ؽ�ó�� �ε��ϰ� ��Ʈ�� �����´�
	return LoadTexture(device, textureFilename);
}

void FontClass::Shutdown()
{
	//��Ʈ �ؽ�ó ��ȯ
	ReleaseTexture();

	//��Ʈ ������ ��ȯ
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

	//vertexPtr�� vertices�� VertexType���� ���� ��ȯ�Ͽ� ����
	vertexPtr = (VertexType*)vertices;

	//������ ���̸� ����
	numLetters = (int)strlen(sentence);

	index = 0;

	//ȭ�鿡 �� ������� �׸�
	for ( i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		//���� ���ڰ� �����̸� �ȼ��� �̵���Ŵ
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			//ù��°
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY -16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			//�ι�°
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, (drawY - 16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			//drawX ��ġ�� ����ũ�⺸�� ���� ũ�� ����
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}

bool FontClass::LoadFontData(char * filename)
{
	ifstream fin;
	int i;
	char temp;

	//m_Font ����
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	//��Ʈ ������� ���ڻ����� ������ �о
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	//95���� �ƽ�Ű�ڵ� ���ڵ��� ����
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

	//������ ����
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	//��Ʈ ������ �迭 ��ȯ
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}
}

bool FontClass::LoadTexture(ID3D11Device *device , WCHAR *filename)
{
	//m_Texture ����
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//Texture��ü �ʱ�ȭ
	return m_Texture->Initialize(device, filename);
}

void FontClass::ReleaseTexture()
{
	//�ؽ�ó ��ü ��ȯ
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}
