// 基本的な頂点シェーダー
#include "BasicShader.hlsli"
#include "InputStructure.hlsli"

PS_Input main(VS_Input input)
{
    PS_Input output;
    
    // クリップ空間への変換
    output.position = mul(float4(input.position, 1.0f), WVPMat);
    
    // ワールド座標の計算
    output.worldPos = mul(float4(input.position, 1.0f), World).xyz;
    
    // ワールド空間の法線計算
    output.worldNormal = mul(input.normal, (float3x3) WorldInvTranspose);
    output.worldNormal = normalize(output.worldNormal); // 正規化
    
    // uvはそのまま
    output.uv = input.uv;
    
    return output;
}
