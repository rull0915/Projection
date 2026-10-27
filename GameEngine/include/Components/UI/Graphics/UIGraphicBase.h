/**
 * @file    BaseGraphic.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/23
 *
 * @brief   グラフィックコンポーネントの基底クラス
 */

#pragma once

#include <SimpleMath.h>

#include "Components/UI/UIComponentBase.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;
	class RectTransform;

	/**
	 * @brief UI空間のグラフィックコンポーネントの基底
	 */
	class UIGraphicBase : public UIComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(UIComponentBase);

	private:

		/** @brief 自身のRectTransformへのポインタ */
		RectTransform* m_pRectTransform;

		/** @brief 乗算色 */
		DirectX::SimpleMath::Color m_color;

		/** @brief サブ乗算色 ボタンなどで使用 */
		DirectX::SimpleMath::Color m_mulColor;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param owner 所有者のポインタ
		 */
		UIGraphicBase(IComponentOwner* owner);

		virtual ~UIGraphicBase() = default;

		/// @}

		/**
		 * @brief 描画を行う
		 */
		virtual void Draw(Renderer& renderer) = 0;

		/**
		 * @brief RectTransformを取得する
		 */
		RectTransform* GetRectTransform() const { return m_pRectTransform; }

		/**
		 * @brief 乗算色を変更する
		 */
		void SetColor(DirectX::SimpleMath::Color color) { m_color = color; }

		/**
		 * @brief サブ乗算色を変更する
		 */
		void SetMulColor(DirectX::SimpleMath::Color color) { m_mulColor = color; }

	protected:

		/** @brief 乗算色を取得する */
		DirectX::SimpleMath::Color GetColor() const { return m_color; }

		/** @brief サブ乗算色を取得する */
		DirectX::SimpleMath::Color GetMulColor() const { return m_mulColor; }
	};
} // namespace RHEngine
