/**
 * @file    GuizmoManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/17
 *
 * @brief   ギズモ表示の管理クラス
 */

#pragma once

#ifdef ENGINE_GUI

#include <SimpleMath.h>

#include "Components/World/Transform/Transform.h"
#include "Editor/Editor/SceneCamera.h"

namespace RHEngine
{
	/**
	 * @brief ギズモ表示管理クラス
	 */
	class GuizmoManager
	{
	public:
		/** @brief 移動ギズモ表示フラグのビット */
		static constexpr int DRAW_TRANSLATION = 1 << 0;

		/** @brief 回転ギズモ表示フラグのビット */
		static constexpr int DRAW_ROTATION = 1 << 1;

		/** @brief 拡縮ギズモ表示フラグのビット */
		static constexpr int DRAW_SCALE = 1 << 2;

	private:
		/** @brief 描画フラグ */
		int m_drawFlag = DRAW_TRANSLATION;

	public:

		/**
		 * @brief ギズモを表示する
		 * @param camera シーンビューのカメラポインタ
		 * @param pTransform 操作対象
		 * @param pos 表示領域の左上
		 * @param size 表示領域のサイズ
		 */
		void DrawTransformGuizmo(SceneCamera* pCamera, Transform* pTransform, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 size);

		/**
		 * @brief 描画フラグの変更
		 * @param flag 変更後のフラグ
		 */
		void SetDrawFlag(int flag) { m_drawFlag = flag; }
	};
}	// namespace RHEngine

#endif // ENGINE_GUI
