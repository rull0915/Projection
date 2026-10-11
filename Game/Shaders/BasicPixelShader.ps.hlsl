#include "BasicShader.hlsli"
#include "InputStructure.hlsli"

cbuffer PerMaterial : register(b2)
{
    float4 baseColor; // ベースカラー
};

float4 main(PS_Input input) : SV_TARGET
{
    // 平行光源の取得
    DirectionalLight dirLight = light.directionalLights[0];
    
    // ライトの向き 光の到来方向は進行方向の逆
    float3 lightDir = normalize(-dirLight.direction);
    
    // 拡散反射の計算
    float NdotL = saturate(dot(input.worldNormal, lightDir));
    
    // ライティングの合成
    float3 ambient = float3(0.2f, 0.2f, 0.2f);  // テスト用環境光
    
    // ディフューズ光の計算
    float3 diffuse = dirLight.color.rgb * dirLight.intensity * NdotL;
    
    // 最終的な色 = ベースカラー * (環境光 + ディレクショナル光)
    float3 finalColor = baseColor.rgb * (ambient + diffuse);
    
    return float4(finalColor, baseColor.a);
}
