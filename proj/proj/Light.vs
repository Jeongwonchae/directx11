cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

//Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;

	//������ ��� ����� ���� ��ġ ���͸� 4 ������ ����
	input.position.w = 1.0f;

	//����, �� �� ���� ��Ŀ� ���� ������ ��ġ�� ���
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//�ȼ� ���̴��� �ؽ��� ��ǥ�� ����
	output.tex = input.tex;

	//���� ��Ŀ� ���ؼ��� ���� ���͸� �����
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//���� ���͸� ����ȭ
	output.normal = normalize(output.normal);

	return output;
}