#define _CRT_SECURE_NO_WARNINGS
#include "Stdafx.h"
#include "FontClass.h"
#include "FontShaderClass.h"
#include "TextClass.h"

TextClass::TextClass()
{
}

TextClass::TextClass(const TextClass &)
{
}


TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device *device, ID3D11DeviceContext * deviceContext, HWND hwnd, int screenWidth, int screenHeight, XMMATRIX baseViewMatrix)
{
	bool result;

	//가로 세로 값 저장
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_baseViewMatrix = baseViewMatrix;

	//폰트 객체 생성
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	//폰트 객체 초기화
	char* txtFilename = new char[sizeof("../proj/data/fontdata.txt")];
	WCHAR* ddsFilename = new WCHAR[sizeof("../proj/data/font.dds")];
	strcpy(txtFilename, "../proj/data/fontdata.txt");
	wcscpy(ddsFilename, L"../proj/data/font.dds");
	result = m_Font->Initialize(device, txtFilename, ddsFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object", L"Error", MB_OK);
		return false;
	}

	//폰트 셰이더 객체 생성
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}
	//폰트 객체 초기화
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font Shader object", L"Error", MB_OK);
		return false;
	}

	//첫번째 sentence 초기화
	result = InitializeSentence(&m_sentence1, 16, device);
	if (!result)
	{
		return false;
	}

	//새로운 string 정보와 문장 정점 버퍼를 update
	result = UpdateSentence(m_sentence1, (char*)"Fps : ", 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	//두번째 sentence 초기화
	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result)
	{
		return false;
	}

	//새로운 string 정보와 문장 정점 버퍼를 update
	result = UpdateSentence(m_sentence2, (char*)"Cpu :", 20, 40, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	//sentence1 반환
	ReleaseSentence(&m_sentence1);

	//sentence2 반환
	ReleaseSentence(&m_sentence2);
	
	//폰트 셰이더 객체 반환
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	//폰트 객체 반환
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}
}

bool TextClass::Render(ID3D11DeviceContext *deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix)
{
	bool result;

	//첫번째 문장 그리기
	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	//두번째 문장 그리기
	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext *deviceContext)
{
	//fps를 10000이하로 자름
	if (fps > 9999)
	{
		fps = 9999;
	}

	//fps 정수를 문자열 형식으로 변환
	char tempString[16] = { 0, };
	_itoa_s(fps, tempString, 10);

	//fps 문자열을 설정
	char fpsString[16] = { 0, };
	strcpy_s(fpsString, "Fps : ");
	strcat_s(fpsString, tempString);

	float red = 0;
	float green = 0;
	float blue = 0;

	//fps가 60 이상이면 fps 색상을 녹색으로 설정
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	return UpdateSentence(m_sentence1, fpsString, 20, 20, red, green, blue, deviceContext);
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext *deviceContext)
{
	char tempString[16] = { 0, };
	_itoa_s(cpu, tempString, 10);

	//fps 문자열을 설정
	char cpuString[16] = { 0, };
	strcpy_s(cpuString, "Cpu : ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");
	
	return UpdateSentence(m_sentence2, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext *deviceContext)
{
	//mouseX 정수를 문자열 형식으로 변환
	char tempString[16] = { 0, };
	_itoa_s(mouseX, tempString, 10);

	//mouseX 문자열을 설정
	char mouseString[16] = { 0, };
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	//문장 정점 버퍼를 새 문자열 정보로 업데이트
	if (!UpdateSentence(m_sentence1, mouseString, 20, 20, 1.0f, 1.0f, 10.f, deviceContext))
	{
		return false;
	}

	//mouseY 정수를 문자열 형식으로 변환
	_itoa_s(mouseY, tempString, 10);

	//mouseY 문자열을 설정
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);
	if (!UpdateSentence(m_sentence2, mouseString, 20, 40, 1.0f, 1.0f, 10.f, deviceContext))
	{
		return false;
	}
	return true;
}

bool TextClass::InitializeSentence(SentenceType **sentence, int maxLength, ID3D11Device *device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	(*sentence)->maxLength = maxLength;

	(*sentence)->vertexCount = 6 * maxLength;

	(*sentence)->indexCount = (*sentence)->vertexCount;

	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType)* (*sentence)->vertexCount));

	for ( i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*(*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType *sentence, char *text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext *deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	numLetters = (int)strlen(text);

	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType)*sentence->vertexCount));

	drawX = (float)(((m_screenWidth / 2)* -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)* sentence->vertexCount));

	deviceContext->Unmap(sentence->vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType **sentence)
{
	if (*sentence)
	{
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		delete *sentence;
		*sentence = 0;
	}
}

bool TextClass::RenderSentence(ID3D11DeviceContext *deviceContext, SentenceType *sentence, XMMATRIX wolrdMatrix, XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	result = m_FontShader->Render(deviceContext, sentence->indexCount, wolrdMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		return false;
	}
	
	return true;
}
