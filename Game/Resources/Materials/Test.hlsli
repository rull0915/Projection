struct VS_INPUT
{
    float3 Position : POSITION; // 頂点座標 (x, y, z, w)
    float4 Color : COLOR; // 頂点カラー (r, g, b, a)
    float2 Tex      : TEXCOORD; // UV座標 (x, y)
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};
