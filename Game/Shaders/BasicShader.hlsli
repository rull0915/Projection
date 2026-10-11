// ゲーム全体で使用する基本的な構造

struct PointLight
{
    float3 pos; // ライトの位置
    float radius; // 光が届く最大の半径
    float3 color; // 光の色
    float intensity; // 光の強さ
};

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

struct DirectionalLight
{
    float3 direction; // ライトの向き
    float intensity; // ライトの強さ
    float3 color; // ライトの色
    float padding;
};

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
    CameraInfo camera;
    LightInfo light;
    TimeInfo time;
}

cbuffer PerObject : register(b1)
{
    matrix World;
    matrix WVPMat;
    matrix WorldInvTranspose;
}
