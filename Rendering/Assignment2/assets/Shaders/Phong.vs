
cbuffer PerFrameBuffer : register(b0)
{
	matrix WorldToViewMatrix;
	matrix ProjectionMatrix;

	float4 LightPosition;
	float4 CameraPosition;

	bool   IsDirectionalLight;
	float3 Pad;
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
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
	float3 LightDir : LIGHTDIR;
	float3 ViewDir : VIEWDIR;
};

PSIn VS_main(VSIn input)
{
	PSIn output = (PSIn)0;
	
	// model-to-view
	matrix MV = mul(ModelToWorldMatrix, WorldToViewMatrix);
	// model-to-projection
	matrix MVP = mul(MV, ProjectionMatrix);
	
	output.Pos = mul(float4(input.Pos, 1), MVP);
	output.TexCoord = input.TexCoord;

	// Phong
	output.Normal = mul(input.Normal, ModelToWorldMatrix);

	float3 worldPos =  mul(float4(input.Pos, 1), ModelToWorldMatrix).xyz;

	if (IsDirectionalLight) {
		output.LightDir = normalize(LightPosition.xyz);
	} else {
		output.LightDir = normalize(  LightPosition.xyz - worldPos );
	}

	output.ViewDir = normalize(worldPos - CameraPosition.xyz );

	return output;
}