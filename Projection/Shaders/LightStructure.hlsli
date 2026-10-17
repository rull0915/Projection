// ライトの構造体を定義するヘッダーファイル
#ifndef LIGHT_STRUCTURE_HLSLI
#define LIGHT_STRUCTURE_HLSLI

// ポイントライト
struct PointLight
{
    float3 pos; // ライトの位置
    float radius; // 光が届く最大の半径
    float3 color; // 光の色
    float intensity; // 光の強さ
};

// スポットライト
struct SpotLight
{
    float3 pos; // ライトの位置
    float length; // ライトが届く最大距離
    float3 color; // ライトの色    
    float intensity; // ライトの強度
    float3 direction; // ライトの向き
    float cosInnerAngle;
    float cosOuterAngle;
    float3 padding;
};

// ディレクショナルライト
struct DirectionalLight
{
    float3 direction; // ライトの向き
    float intensity; // ライトの強さ
    float3 color; // ライトの色
    float padding;
};

// 影の設定
struct ShadowInfo
{
    matrix LightVP;
    float ShadowBias;
    float3 padding;
};

#endif
