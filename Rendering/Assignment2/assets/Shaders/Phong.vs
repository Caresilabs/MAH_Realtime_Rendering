
cbuffer PerFrameBuffer : register(b0)
{
	matrix WorldToViewMatrix;
	matrix ProjectionMatrix;

	float4 LightPosition;
	float4 CameraPosition;

	matrix LightProjectionMatrix;
	matrix LightToViewMatrix;

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
	float3x3 Normals : NORMALS;
	float2 TexCoord : TEX;
	float3 LightPos : LIGHTPOS;
	float3 WorldPos : WORLDPOS;
	float3 CameraPos : CAMERAPOS;
	float4 LightPosTest : LIGHTPOSTEST;
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

	float4 wpos = mul(float4(input.Pos, 1), ModelToWorldMatrix);

	// light Pos
	output.LightPosTest = mul( wpos, mul(  LightToViewMatrix, LightProjectionMatrix ));  

	// Normals (TBN)
	output.Normals[0] = mul(input.Tangent, ModelToWorldMatrix);
	output.Normals[1] = mul(input.Binormal, ModelToWorldMatrix);
	output.Normals[2] = mul(input.Normal, ModelToWorldMatrix);

	float3 worldPos =  wpos.xyz;

	if (IsDirectionalLight) {
		output.LightPos = normalize( LightPosition.xyz );
	} else {
		output.LightPos = (  LightPosition.xyz  );
	}

	output.WorldPos = worldPos;
	output.CameraPos = CameraPosition;

	return output;
}