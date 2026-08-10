#include "Test.hlsli"

cbuffer PerFrame : register(b0)
{
	float4x4 View;
	float4x4 Proj;
}

cbuffer PerObject : register(b1)
{
	float4x4 World;
}

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    
    // WVP変換
    output.Position = float4(input.Position, 1);
    output.Position = mul(output.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Proj);
    
    // 頂点カラーをそのままピクセルシェーダーへ送る
    output.Color = input.Color;
    output.Tex = input.Tex;
    
    return output;
}
