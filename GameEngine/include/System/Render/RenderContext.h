//====================================================//
// ファイル名   : RenderContext.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/04
//
// 概要 : 描画コンテキスト
//
// 更新履歴 :
// 2026/07/04 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "RenderTarget.h"
#include "Renderer/Shadow/ShadowMap.h"
#include "Scene/SceneInfo.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//

	namespace DrawFlag
	{
		static constexpr uint8_t World = 1 << 0;
		static constexpr uint8_t UI = 1 << 1;
		static constexpr uint8_t WorldDebug = 1 << 2;
		static constexpr uint8_t UIDebug = 1 << 3;
	}

	// 描画の情報
	struct RenderContext
	{
		// 描画するカメラのデータ
		SceneCameraData cameraData;

		// 背景色
		DirectX::SimpleMath::Color backColor = { 0, 0, 0, 0 };

		// 描画先
		RenderTarget* target = nullptr;

		// 描画フラグ
		uint8_t flags = DrawFlag::World | DrawFlag::UI;

		// 描画時書かれている情報をリセットするかどうか
		bool clearTarget = true;
	};

	// 影用描画の情報
	struct ShadowRenderContext
	{
		// 影を作るライトをカメラとみなしたときのデータ
		SceneCameraData cameraData;
		
		// 描画対象
		ShadowMap* pShadowMap;
	};
}	// namespace REngine
