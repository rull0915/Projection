// ライティング計算の関数を定義するヘッダーファイル
#ifndef LIGHTING_HLSLI
#define LIGHTING_HLSLI

#include "lightStructure.hlsli"

// ディレクショナルライト計算

float3 CalcLembertDiffuse(float3 lightDir, float3 lightColor, float lightIntensity, float3 normal)
{       
    // ライトの方向を正規化
    lightDir = normalize(lightDir);
    
    // 法線とライト方向の内積を計算
    float NdotL = saturate(dot(normal, lightDir));
    
    // Diffuseを計算 : Lambertian反射モデル
    return lightColor.rgb * lightIntensity * NdotL;
}

float3 CalcPhongSpecular(float3 lightDir, float3 lightColor, float lightIntensity, float3 eyePos, float3 worldPos, float3 normal, float specularPower)
{
    // ライトの方向を正規化
    lightDir = normalize(lightDir);
    
    // 反射ベクトルを求める
    float3 refVec = reflect(-lightDir, normal);

    // 光が当たった場所から視点に伸びるベクトルを求める
    float3 toEye = eyePos - worldPos;
    
    // 正規化
    toEye = normalize(toEye);
    
    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);
    
    // 鏡面反射の強さを0以上の数値にする
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, specularPower);
    
    return lightColor.rgb * lightIntensity * t;
}

float3 CalcBlinnPhongSpecular(float3 lightDir, float3 lightColor, float lightIntensity, float3 eyePos, float3 worldPos, float3 normal, float specularPower)
{
    // ライトの向きを正規化
    lightDir = normalize(lightDir);
    
    // 点からカメラへのベクトル
    float3 toEye = normalize(eyePos - worldPos);
    
    // ライト方向と視線方向のハーフベクトルを算出
    float3 halfVec = normalize(lightDir + toEye);
    
    // 法線とハーフベクトルの内積を計算
    float NdotH = saturate(dot(normal, halfVec));
    
    // 鏡面反射強度を計算
    float t = pow(NdotH, specularPower);
    
    return lightColor.rgb * lightIntensity * t;
}

#endif
