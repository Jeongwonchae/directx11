//GLOBAL
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//TYPE
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex: TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

//Vertex Shader
PixelInputType FontVertexShader(VertexInputType input)
{
	PixelInputType output;

	//������ ��� ����� ���� ��ġ ���͸� 4������ ����
	input.position.w = 1.0f;

	//����,  �� �� ���� ��Ŀ� ���� ������ ��ġ�� ���
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//�ȼ� ���̴��� ����� �Է� ������ ����
	output.tex = input.tex;

	return output;
}