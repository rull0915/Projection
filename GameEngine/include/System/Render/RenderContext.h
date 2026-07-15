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
#include "Components/World/Camera/CameraBase.h"
#include "RenderTarget.h"

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
		// 使用するカメラ
		CameraBase* camera = nullptr;

		// 描画先
		RenderTarget* target = nullptr;

		// 背景色
		DirectX::SimpleMath::Color back = { 0, 0, 0, 0 };

		// 描画フラグ
		uint8_t flags = DrawFlag::World | DrawFlag::UI;
	};
}	// namespace REngine
