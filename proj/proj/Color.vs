//GLOBAL
cbuffer MatrixBuffer
{
	mastrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//TYPE
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

//Vertex Shader
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	//������ ��� ����� ���� ��ġ ���͸� 4������ ����
	input.position.w = 1.0f;

	//����,  �� �� ���� ��Ŀ� ���� ������ ��ġ�� ���
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectiondMatrix);

	//�ȼ� ���̴��� ����� �Է� ������ ����
	output.color = input.color;

	return output;
}