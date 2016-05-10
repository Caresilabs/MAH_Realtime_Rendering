cbuffer PerFrameBuffer : register(b0)
{
	matrix WorldToViewMatrix;
	matrix ProjectionMatrix;
};

cbuffer PerObjectBuffer : register(b1)
{
	matrix ModelToWorldMatrix;
};

struct VSIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float2 TexCoord : TEX;
};

struct PSIn
{
	float4 Pos  : SV_Position;
};

PSIn VS_main(VSIn input)
{
	PSIn output = (PSIn)0;
	
	// model-to-view
	matrix MV = mul(ModelToWorldMatrix, WorldToViewMatrix);
	// model-to-projection
	matrix MVP = mul(MV, ProjectionMatrix);
	
	output.Pos = mul(float4(input.Pos, 1), MVP);

	return output;
}