cbuffer ConstBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float3 lightDir;
};

struct VSInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float3 normal : TEXCOORD0;
};
