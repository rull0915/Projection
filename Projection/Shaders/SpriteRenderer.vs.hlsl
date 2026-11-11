#include "BasicShader.hlsli"
#include "InputStructure.hlsli"

cbuffer SpriteInfo : register(b3)
{
    float4 uv : TEXCOORD0;
};

PS_Input main(VS_Input input)
{
    PS_Input output = (PS_Input)0;
          
    // クリップ空間への変換
    output.position = mul(float4(input.position, 1.0f), WVPMat);
    
    // ワールド座標の計算
    output.worldPos = mul(float4(input.position, 1.0f), World).xyz;
    
    // ワールド空間の法線計算
    output.worldNormal = mul(input.normal, (float3x3) WorldInvTranspose);
    output.worldNormal = normalize(output.worldNormal); // 正規化
    
    // uvを計算
    float2 size = uv.zw - uv.xy;
    output.uv = uv.xy + input.uv * size;
    
    return output;
}
