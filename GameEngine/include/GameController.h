/**
 * @file    GameController.h
 * @author  Hoshino Ryunosuke
 * @date    2026/09/01
 *
 * @brief   ゲームを操作するクラス
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief ゲームを操作する
	 * @details 現状は終了処理をどこからでも呼び出すためのラップ関数のみを実装している
	 */
	class GameController
	{
	private:

		/// @name ライフサイクル
		/// @{

		GameController() = default;

		~GameController() = default;

		/// @}

	public:

		/**
		 * @brief インスタンスを取得する
		 * @return 静的なインスタンス
		 */
		static GameController& Instance()
		{
			static GameController instance;
			return instance;
		}

		/**
		 * @brief ゲームを終了する
		 */
		void Quit();
	};
}	// namespace RHEngine
