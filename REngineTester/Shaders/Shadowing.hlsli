// シャドウマップ計算の関数を定義するヘッダーファイル
#ifndef SHADOWING_HLSLI
#define SHADOWING_HLSLI

float CalcShadow(
    float3 position,            // ピクセルのワールド座標
    matrix lightVP,             // ライトのVP行列
    Texture2D shadowMap,        // シャドウマップ
    SamplerState shadowSampler // サンプラー
)
{       
    // ライト空間へ
    float4 lightSpacePos = mul(float4(position, 1.0f), lightVP);
    
    // 透視除算
    float3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    
    // NDCからUVへ変換
    float2 shadowUV = float2(projCoords.x * 0.5f + 0.5f, -projCoords.y * 0.5f + 0.5f);
    
    // シャドウマップの範囲外なら影なし
    if (shadowUV.x < 0.0f || shadowUV.y < 0.0f || shadowUV.x > 1.0f || shadowUV.y > 1.0f)
        return 1.0f;
    
    // 描画対象ピクセルの深度を取得
    float currentDepth = projCoords.z;
    
    // シャドウマップに記されている深度を取得
    float depth = shadowMap.Sample(shadowSampler, shadowUV).r;
    
    // 影判定
    if (currentDepth > depth + 0.001f)
    {
        return 0.3f; // 影の中
    }
    
    return 1.0f;
}

#endif
