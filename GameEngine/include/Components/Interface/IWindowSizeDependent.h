/**
 * @file    IWindowSizeDependent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/22
 *
 * @brief   ウィンドウサイズに依存することを明示するインターフェース
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief ウィンドウサイズ依存コンポーネントのインターフェース
	 */
	class IWindowSizeDependent
	{
	public:

		/// @name ライフサイクル
		/// @{
		
		IWindowSizeDependent() = default;

		virtual ~IWindowSizeDependent() = default;

		/// @}

		/**
		 * @brief ウィンドウサイズの変更を受け取る
		 * @param width 変更後のウィンドウ幅
		 * @param height 変更後のウィンドウ高さ
		 */
		virtual void OnWindowSizeChanged(int width, int height) = 0;
	};
}	// namespace RHEngine
