/**
 * @file    RendererInfo.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/11
 *
 * @brief   描画に必要な構造体をまとめたヘッダ
 */

#pragma once

#include <SimpleMath.h>

#include "Common/LightData.h"

namespace RHEngine
{
	/** @brief ポイントライトの最大数 */
	static const int MAX_POINT_LIGHT = 3;

	/** @brief スポットライトの最大数 */
	static const int MAX_SPOT_LIGHT = 3;

	/** @brief ディレクショナルライトの最大数 */
	static const int MAX_DIRECTIONAL_LIGHT = 3;

	/**
	 * @brief ライト情報
	 */
	struct alignas(16) LightInfo
	{
		PointLight pointLights[MAX_POINT_LIGHT];					///< ポイントライト
		SpotLight spotLights[MAX_SPOT_LIGHT];						///< スポットライト
		DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT];	///< ディレクショナルライト

		int pointLightCount;		///< 有効なポイントライトの数
		int spotLightCount;			///< 有効なスポットライトの数
		int directionalLightCount;	///< 有効なディレクショナルライトの数

		float padding;
	};

	/**
	 * @brief 時間情報
	 */
	struct alignas(16) TimeInfo
	{
		float time;			///< プロジェクト開始からの経過時間
		float elapsedTime;	///< 1fの経過時間
		DirectX::SimpleMath::Vector2 padding;
	};

	/**
	 * @brief 影情報
	 */
	struct alignas(16) ShadowInfo
	{
		DirectX::SimpleMath::Matrix lightVP;	///< ライトのVP行列
		float shadowBias;						///< シャドウバイアス
		DirectX::SimpleMath::Vector3 padding;
	};

	/**
	 * @brief フレーム毎に更新されるバッファ
	 */
	struct alignas(16) PerFrame
	{
		LightInfo light;	///< ライト情報
		TimeInfo time;		///< 時間情報
		ShadowInfo shadow;	///< 影情報
	};

	/**
	 * @brief カメラ毎に更新されるバッファ
	 */
	struct alignas(16) PerCamera
	{
		DirectX::SimpleMath::Matrix view;	///< ビュー行列
		DirectX::SimpleMath::Matrix proj;	///< 射影行列
		DirectX::SimpleMath::Matrix vpMat;	///< ビュー×射影行列
		DirectX::SimpleMath::Matrix invVpMat;	///< ビュー×射影の逆行列
		DirectX::SimpleMath::Vector3 cameraPos;	///< カメラのワールド座標
		float padding;
	};

	/**
	 * @brief オブジェクト毎に更新されるバッファ
	 */
	struct alignas(16) PerObject
	{
		DirectX::SimpleMath::Matrix world;	///< ワールド行列
		DirectX::SimpleMath::Matrix wvp;	///< ワールド × ビュー × 射影行列
		DirectX::SimpleMath::Matrix worldInvTranspose;	///< 逆転置ワールド行列
	};
}	// namespace RHEngine
