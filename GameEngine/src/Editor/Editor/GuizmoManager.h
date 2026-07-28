//====================================================//
// ファイル名   : GuizmoManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/17
//
// 概要 : ギズモ表示の管理クラス
//
// 更新履歴 :
// 2026/07/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/Transform/Transform.h"
#include "Components/World/Camera/CameraBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class GuizmoManager
	{
	public:
		static constexpr int DRAW_TRANSLATION = 1 << 0;
		static constexpr int DRAW_ROTATION = 1 << 1;
		static constexpr int DRAW_SCALE = 1 << 2;

	private:
		// 描画フラグ
		int m_drawFlag = DRAW_TRANSLATION;

	public:
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void DrawTransformGuizmo(CameraBase* camera, Transform* transform, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 size);

		void SetDrawFlag(int flag) { m_drawFlag = flag; }
	};
}	// namespace REngine
