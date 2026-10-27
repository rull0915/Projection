/**
 * @file    UIBehaviorBase.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   動きを持たせるUIの基盤
 */

#pragma once

#include "Components/UI/UIComponentBase.h"

namespace RHEngine
{
	class IComponentOwner;

	/**
	 * @brief 動きを持つUIコンポーネントの基底
	 */
	class UIBehaviorBase : public UIComponentBase
	{
	public:
		DECLARE_COMPONENT_TYPE(UIComponentBase);

	private:

		/** @brief マウスカーソルが自身に重なっているかどうか */
		bool m_isHovered;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		UIBehaviorBase(IComponentOwner* pOwner)
			: UIComponentBase(pOwner)
			, m_isHovered{ false }
		{}

		~UIBehaviorBase() = default;

		/// @}

		/** @brief マウスクリック開始時 */
		virtual void OnMouseDown() {};

		/** @brief マウスクリック終了時 */
		virtual void OnMouseUp() {};

		/** @brief ホバー状態のセット */
		void SetHovered(bool hovered) { m_isHovered = hovered; };

		/** @brief ホバー中かどうかの取得 */
		bool IsHovered() const { return m_isHovered; }
	};
} // namespace RHEngine
