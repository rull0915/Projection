/**
 * @file    IComponentListener.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/08
 *
 * @brief   コンポーネントの追加通知を受け取るインターフェース
 */

#pragma once

namespace RHEngine
{
	class ComponentBase;

	/**
	 * @brief コンポーネントの追加、削除通知を受け取るインターフェース
	 */
	class IComponentListener
	{
	public:

		/// @name ライフサイクル
		/// @{

		IComponentListener() = default;

		virtual ~IComponentListener() = default;

		/// @}

		/**
		 * @brief コンポーネントの追加が通知される
		 * @param  追加されたコンポーネントのポインタ
		 */
		virtual void OnComponentAdded(ComponentBase*) {}

		/**
		 * @brief コンポーネントの削除が通知される
		 * @param  削除されるコンポーネントのポインタ
		 */
		virtual void OnComponentRemoved(ComponentBase*) {}
	};
} // namespace RHEngine
