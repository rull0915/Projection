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


namespace REngine
{
	//====================================================//
	// 構造体宣言
	//====================================================//

	struct PointLight
	{
		DirectX::SimpleMath::Vector3 pos;	// ライトの位置
		float radius;						// 光が届く最大の半径
		DirectX::SimpleMath::Vector3 color; // 光の色
		float intensity;					// 光の強さ
	};

	struct SpotLight
	{
		DirectX::SimpleMath::Vector3 pos;	// ライトの位置
		float length;						// ライトが届く最大距離
		DirectX::SimpleMath::Vector3 color; // ライトの色    
		float intensity;					// ライトの強度
		DirectX::SimpleMath::Vector3 direction; // ライトの向き
		float cosInnerAngle;
		float cosOuterAngle;
		DirectX::SimpleMath::Vector3 padding;
	};

	struct DirectionalLight
	{
		DirectX::SimpleMath::Vector3 direction; // ライトの向き
		float intensity;						// ライトの強さ
		DirectX::SimpleMath::Vector3 color;		// ライトの色
		float padding;
	};

	static const int MAX_POINT_LIGHT = 3;
	static const int MAX_SPOT_LIGHT = 3;
	static const int MAX_DIRECTIONAL_LIGHT = 3;

	// ライト
	struct LightInfo
	{
		PointLight pointLights[MAX_POINT_LIGHT];					// ポイントライト  
		SpotLight spotLights[MAX_SPOT_LIGHT];						// スポットライト  
		DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT];	// ディレクショナルライト

		int pointLightCount;		// 有効なポイントライトの数
		int spotLightCount;			// 有効なスポットライトの数
		int directionalLightCount;	// 有効なディレクショナルライトの数

		float padding;
	};

	// カメラ
	struct CameraInfo
	{
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix proj;
		DirectX::SimpleMath::Matrix vpMat;
		DirectX::SimpleMath::Matrix invVpMat;
		DirectX::SimpleMath::Vector3 cameraPos;
		float padding;
	};

	// 時間
	struct TimeInfo
	{
		float time;
		float elapsedTime;
	};

	// フレームの情報
	struct FrameInfo
	{
		CameraInfo camera;
		LightInfo light;
		TimeInfo time;
	};
}
