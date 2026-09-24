struct VS_INPUT
{
    float3 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float4 Tangent : TANGENT;
    uint Color : COLOR;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};
