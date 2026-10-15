// ゲーム全体で使用する基本的な構造
#ifndef BASIC_SHADER_HLSLI
#define BASIC_SHADER_HLSLI

#include "LightStructure.hlsli"

static const int MAX_POINT_LIGHT = 3;
static const int MAX_SPOT_LIGHT = 3;
static const int MAX_DIRECTIONAL_LIGHT = 3;

struct LightInfo
{
    PointLight pointLights[MAX_POINT_LIGHT]; // ポイントライト  
    SpotLight spotLights[MAX_SPOT_LIGHT]; // スポットライト  
    DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT]; // ディレクショナルライト
    
    int pointLightCount; // 有効なポイントライトの数
    int spotLightCount; // 有効なスポットライトの数
    int directionalLightCount; // 有効なディレクショナルライトの数
    
    float padding;
};

struct CameraInfo
{
    matrix View; // ビュー行列
    matrix Proj; // プロジェクション行列   
    matrix VPMat; // ビュー * プロジェクション 行列
    matrix InvVP; // 逆 ビュー * プロジェクション行列
    float3 cameraPos; // カメラの位置
    float padding; // 16byteに合わせるためのパディング
};

struct TimeInfo
{
    float time;
    float deltaTime;
    float2 padding;
};

cbuffer PerFrame : register(b0)
{
    LightInfo light;
    TimeInfo time;
    ShadowInfo shadowInfo;
}

cbuffer PerCamera : register(b1)
{
    CameraInfo camera;
}

cbuffer PerObject : register(b2)
{
    matrix World;
    matrix WVPMat;
    matrix WorldInvTranspose;
}

// シャドウマップ


#endif
