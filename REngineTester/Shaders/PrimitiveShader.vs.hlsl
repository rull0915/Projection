#include "BasicShader.hlsli"

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION; 
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.color = input.color;
    output.uv = input.uv;
    output.position = mul(float4(input.position, 1.0f), WVPMat);

    return output;
}
