#include "Test.hlsli"

cbuffer Pixel : register(b2)
{
    float time;
    float4 mulColor;
}

Texture2D tex : register(t0);
SamplerState sam : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.Color * time;
}
