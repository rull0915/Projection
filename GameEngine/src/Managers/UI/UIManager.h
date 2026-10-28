/**
 * @file    UIManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/24
 *
 * @brief   UI管理クラス
 */

#pragma once

#include "../ReserveContainer.h"
#include "Components/UI/Canvas/Canvas.h"

namespace RHEngine
{
	class Renderer;

	/**
	 * @brief UI管理クラス Canvasを保持し、描画や更新を行う
	 */
	class UIManager : public ReserveContainer<Canvas>
	{
	public:

		/// @name ライフサイクル
		/// @{

		UIManager();

		~UIManager();

		/// @}

		/**
		 * @brief マウスの入力を調べて全キャンバスに通知する
		 */
		void CheckEvent();

		/**
		 * @brief 全てのキャンバスを描画する
		 * @param renderer 
		 */
		void Draw(Renderer& renderer);

		/**
		 * @brief 特定の点と当たるCanvasを調べ、衝突時イベントを呼び出す
		 * @param position 
		 */
		void CheckHitRay(DirectX::SimpleMath::Vector2 position);

		/**
		 * @brief デバッグ描画
		 * @param renderer 
		 * @param color 
		 */
		void DebugDraw(Renderer& renderer, DirectX::SimpleMath::Color color);

	private:

		/**
		 * @brief キャンバスを描画順にソートする
		 */
		void SortCanvas();
	};
}	// namespace RHEngine
