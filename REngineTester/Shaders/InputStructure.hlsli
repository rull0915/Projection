// 入力構造体をまとめたヘッダ
#ifndef INPUT_STRUCTURE_HLSLI
#define INPUT_STRUCTURE_HLSLI

// VertexShader
struct VS_Input
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

// PixelShader
struct PS_Input
{
    float4 position : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 worldNormal : TEXCOORD1;
    float2 uv : TEXCOOR2;
};

#endif
