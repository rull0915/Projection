#include "BasicShader.hlsli"
#include "InputStructure.hlsli"

cbuffer Color : register(b3)
{
    float4 color;
}

///< 10番以降にすることでマテリアルによるシリアライズ対象から外す
Texture2D sprite : register(t11);   
SamplerState spriteSampler : register(s11);

float4 main(PS_Input input) : SV_TARGET
{
    float4 baseColor = sprite.Sample(spriteSampler, input.uv);
    
    return baseColor * color;
}
