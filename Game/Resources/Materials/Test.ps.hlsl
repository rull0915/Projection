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
    // タイマーでテクスチャの位置を動かす
    float2 t = input.Tex;
    t.x += time;
    
    // テクスチャだけの色を取得
    float4 texColor = tex.Sample(sam, t);
    
    // 乗算色を計算
    float4 mul = mulColor;
    
    // 中心からの距離を計算
    float2 p = input.Tex - 0.5;
    float len = length(p);
    
    // 近いほど1に近くなる
    float ex = exp(-3 * abs(len));
    
    float co = abs(cos(len * 20));
    
    mul.rgb *= (ex * co);
    
    return texColor * mul;
}
