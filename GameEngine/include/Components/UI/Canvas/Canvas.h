/**
 * @file    Canvas.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/27
 *
 * @brief   キャンバスコンポーネント
 */

#pragma once

#include <SimpleMath.h>

#include "Components/UI/UIComponentBase.h"
#include "Components/Interface/IWindowSizeDependent.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;
	class RectTransform;

	/**
	 * @brief キャンバスコンポーネント
	 */
	class Canvas : public UIComponentBase, public IWindowSizeDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(UIComponentBase);

		/**
		 * @brief マウスのイベント列挙型
		 */
		enum class MouseEvent
		{
			Down,	///< クリック開始
			Up		///< クリック終了
		};

	private:

		/** @brief 描画順 大きいほど手前 */
		int m_drawOrder;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		Canvas(IComponentOwner* pOwner);

		~Canvas() = default;

		/// @}

		/**
		 * @brief 描画関数 自身の子を再帰的に描画していく
		 */
		void Draw(Renderer& renderer);

		/**
		 * @brief 自身の子供の中で特定の点と重なるオブジェクトを取得する
		 * @param point 調べるスクリーン座標
		 * @return 当たっていたRectTransform
		 */
		RectTransform* CheckHit(DirectX::SimpleMath::Vector2 point);

		/**
		 * @brief マウスのイベントを子に通知する
		 * @param event イベントタイプ
		 */
		void NotifyMouseEvent(MouseEvent event);

		/**
		 * @brief ウィンドウサイズ変更時の処理
		 * @details 自分のRectTransformのサイズを変更する
		 * @param width 変更後の幅
		 * @param height 変更後の高さ
		 */
		void OnWindowSizeChanged(int width, int height) override;

		/// @name ゲッター
		/// @{
		
		/** @brief 描画順 */
		int GetDrawOrder() const
		{
			return m_drawOrder;
		}

		/// @}

		/// @name セッター
		/// @{

		/** @brief 描画順 */
		void SetDrawOrder(int order)
		{
			m_drawOrder = order;
		}

		/// @}

	private:

		/**
		 * @brief 自身の子供を再帰的に描画する
		 * @param rectTransform 親のRectTransform
		 */
		void DrawChild(RectTransform* pRectTransform, Renderer& renderer);

		/**
		 * @brief 特定の点が重なっているかどうかを再帰的に調べる
		 * @param pRectTransform 親のRectTransform
		 * @return 重なっているRect
		 */
		RectTransform* CheckHitPoint(RectTransform* pRectTransform, DirectX::SimpleMath::Vector2 point);

		/**
		 * @brief マウスイベントを再帰的に通知する
		 * @param pRectTransform 親のRect
		 * @param event マウスイベント
		 */
		void MouseEventChild(RectTransform* pRectTransform, MouseEvent event);
	};
}	// namespace RHEngine
