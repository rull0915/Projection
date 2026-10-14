#pragma once

#include "BasicShader.hlsli"
#include "InputStructure.hlsli"
#include "Lighting.hlsli"

cbuffer PerMaterial : register(b2)
{
    float4 BaseColor;           // ベースカラー
    float4 SpecularColor;       // スペキュラカラー
    float DiffuseIntensity;     // 拡散反射率
    float SpecularIntensity;    // 鏡面反射率
    float Shininess;            // ハイライトの鋭さ
};

float4 main(PS_Input input) : SV_TARGET
{
    // 法線を正規化
    float3 normal = normalize(input.worldNormal);
    
    // ディフューズの合計値
    float3 totalDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 totalSpecular = float3(0.0f, 0.0f, 0.0f);
    
    ///-------------- ポイントライトの計算 --------------///
    for (int i = 0; i < light.pointLightCount; i++)
    {
        // ポイントライトを取得
        PointLight pLight = light.pointLights[i];
        
        // ライトの向きを計算
        float3 lDir = pLight.pos - input.worldPos;
        
        // ディフューズの計算
        float3 diffuse = CalcLembertDiffuse(lDir, pLight.color, pLight.intensity, normal);

        // スペキュラの計算
        float3 specular = CalcBlinnPhongSpecular(lDir, pLight.color, pLight.intensity, camera.cameraPos, input.worldPos, normal, Shininess);
        
        // 距離を取得
        float distance = length(pLight.pos - input.worldPos);
        
        // 距離の割合を計算
        float affect = max(0.0f, 1.0f - 1.0f / pLight.radius * distance);

        // 影響を指数関数化
        affect = pow(affect, 3.0f);
        
        // 最終的な値を加算する
        totalDiffuse += affect * diffuse;
        totalSpecular += affect * specular;
    }
    
    ///-------------- ディレクショナルライトの計算 --------------///    
    
    for (int j = 0; j < light.directionalLightCount; j++)
    {
        // 平行光源の取得
        DirectionalLight dLight = light.directionalLights[j];
    
        // ディフューズの加算
        totalDiffuse += CalcLembertDiffuse(-dLight.direction, dLight.color, dLight.intensity, normal);
    
        // スペキュラの加算
        totalSpecular += CalcBlinnPhongSpecular(-dLight.direction, dLight.color, dLight.intensity, camera.cameraPos, input.worldPos, normal, Shininess);
    }
    
    // スポットライトの計算
    for (int s = 0; s < light.spotLightCount; s++)
    {
        // ポイントライトを取得
        SpotLight sLight = light.spotLights[s];
        
        // ライトの向きを計算
        float3 lDir = sLight.pos - input.worldPos;
        
        // ディフューズの計算
        float3 diffuse = CalcLembertDiffuse(lDir, sLight.color, sLight.intensity, normal);

        // スペキュラの計算
        float3 specular = CalcBlinnPhongSpecular(lDir, sLight.color, sLight.intensity, camera.cameraPos, input.worldPos, normal, Shininess);
        
        // 距離を取得
        float distance = length(lDir);
        
        // 距離の割合を計算
        float distAffect = max(0.0f, 1.0f - 1.0f / sLight.length * distance);

        // 影響を指数関数化
        distAffect = pow(distAffect, 3.0f);
        
        // ライトの向きを正規化し方向ベクトルに
        float3 dirToLight = normalize(lDir);

        // 照射方向を反転させて内積を計算
        float cosTheta = dot(dirToLight, -normalize(sLight.direction));
        
        // 角度減衰率の計算
        float spotFactor = saturate((cosTheta - sLight.cosOuterAngle) / (sLight.cosInnerAngle - sLight.cosOuterAngle));
        
        // 最終的な影響度
        float totalAffect = distAffect * spotFactor;
        
        // 最終的な値を加算する
        totalDiffuse += totalAffect * diffuse;
        totalSpecular += totalAffect * specular;
    }
    
    // ライティングの合成
    float3 ambient = float3(0.2f, 0.2f, 0.2f); // テスト用環境光
    
    // 最終的な色 = ベースカラー * (環境光 + ディレクショナル光)
    float3 finalColor = 
    BaseColor.rgb * (ambient + DiffuseIntensity * totalDiffuse) +
    SpecularColor.rgb * SpecularIntensity * totalSpecular;
            
    return float4(finalColor, BaseColor.a);
}
