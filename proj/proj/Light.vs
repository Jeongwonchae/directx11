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

	//적절한 행렬 계산을 위해 위치 백터를 4 단위로 변경
	input.position.w = 1.0f;

	//월드, 뷰 및 투영 행렬에 대한 정점의 위치를 계산
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//픽셀 셰이더의 텍스쳐 좌표를 저장
	output.tex = input.tex;

	//월드 행렬에 대해서만 법선 벡터를 계산함
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//법선 벡터를 정규화
	output.normal = normalize(output.normal);

	return output;
}