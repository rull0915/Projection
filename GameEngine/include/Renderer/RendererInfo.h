//====================================================//
// ファイル名   : RendererInfo.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/11
//
// 概要 : 描画に必要な構造体をまとめたヘッダ
//
// 更新履歴 :
// 2026/10/11 新規作成
//====================================================//

#pragma once
#include <SimpleMath.h>

//====================================================//
// インクルードファイル
//====================================================//

#include "Common/LightData.h"

namespace REngine
{
	//====================================================//
	// 構造体宣言
	//====================================================//

	static const int MAX_POINT_LIGHT = 3;
	static const int MAX_SPOT_LIGHT = 3;
	static const int MAX_DIRECTIONAL_LIGHT = 3;

	// ライト
	struct alignas(16) LightInfo
	{
		PointLight pointLights[MAX_POINT_LIGHT];					// ポイントライト  
		SpotLight spotLights[MAX_SPOT_LIGHT];						// スポットライト  
		DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT];	// ディレクショナルライト

		int pointLightCount;		// 有効なポイントライトの数
		int spotLightCount;			// 有効なスポットライトの数
		int directionalLightCount;	// 有効なディレクショナルライトの数

		float padding;
	};

	// 時間
	struct alignas(16) TimeInfo
	{
		float time;
		float elapsedTime;
		DirectX::SimpleMath::Vector2 padding;
	};

	// 影の情報
	struct alignas(16) ShadowInfo
	{
		DirectX::SimpleMath::Matrix lightVP;
		float shadowBias;
		DirectX::SimpleMath::Vector3 padding;
	};

	// フレームの情報
	struct alignas(16) PerFrame
	{
		LightInfo light;
		TimeInfo time;
		ShadowInfo shadow;
	};
	// カメラの情報
	struct alignas(16) PerCamera
	{
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix proj;
		DirectX::SimpleMath::Matrix vpMat;
		DirectX::SimpleMath::Matrix invVpMat;
		DirectX::SimpleMath::Vector3 cameraPos;
		float padding;
	};

	// オブジェクトの情報
	struct alignas(16) PerObject
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix wvp;
		DirectX::SimpleMath::Matrix worldInvTranspose;
	};
}
