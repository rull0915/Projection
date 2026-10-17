Texture2D spriteTexture : register(t0);
SamplerState spriteSampler : register(s0);

float4 main(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_TARGET
{
    return float4(
        texCoord.x,
        texCoord.y,
        0.0f,
        1.0f
    );
}
