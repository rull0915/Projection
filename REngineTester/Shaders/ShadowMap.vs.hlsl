#include "BasicShader.hlsli"

struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float4 posCS : SV_POSITION; // ライト空間のクリップ座標
};

VS_OUTPUT main( VS_INPUT input : POSITION ) : SV_POSITION
{
    VS_OUTPUT output;

    // 頂点変換
    output.posCS = mul(float4(input.position, 1.0f), WVPMat);

    return output;
}
