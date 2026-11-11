#include "BasicShader.hlsli"

struct VSOutput
{
    float3 TexCoord : TEXCOORD0;
    float4 PositionPS : SV_Position;
};

VSOutput main(float4 position : POSITION)
{
    VSOutput vout;

    vout.PositionPS = mul(position, WVPMat);
    vout.PositionPS.z = vout.PositionPS.w; // Draw on far plane
    vout.TexCoord = position.xyz;

    return vout;
}
