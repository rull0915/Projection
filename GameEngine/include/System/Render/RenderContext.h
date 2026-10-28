/**
 * @file    RenderContext.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/04
 *
 * @brief   描画コンテキスト
 */

#pragma once

#include <cstdint>
#include <SimpleMath.h>

#include "RenderTarget.h"
#include "Renderer/Shadow/ShadowMap.h"
#include "Scene/SceneInfo.h"

namespace RHEngine
{
	/**
	 * @brief 描画フラグ
	 */
	namespace DrawFlag
	{
		static constexpr uint8_t World = 1 << 0;
		static constexpr uint8_t UI = 1 << 1;
		static constexpr uint8_t WorldDebug = 1 << 2;
		static constexpr uint8_t UIDebug = 1 << 3;
	}

	/**
	 * @brief 描画をする際の情報をまとめた構造体
	 */
	struct RenderContext
	{
		SceneCameraData cameraData;	///< 描画を行うカメラのデータ

		DirectX::SimpleMath::Color backColor = { 0, 0, 0, 0 };	///< 背景色

		RenderTarget* target = nullptr;	///< 描画先

		uint8_t flags = DrawFlag::World | DrawFlag::UI;	///< 描画フラグ

		bool clearTarget = true;	///< 描画時に描画先をクリアするかどうか
	};

	/**
	 * @brief 影描画の際の情報をまとめた構造体
	 */
	struct ShadowRenderContext
	{
		SceneCameraData cameraData;	///< 影を生成するライトを、カメラとみなしたときのカメラデータ
		
		ShadowMap* pShadowMap;	///< 描画先のシャドウマップ
	};
}	// namespace RHEngine
