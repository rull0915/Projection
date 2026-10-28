/**
 * @file    EngineInitializer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/26
 *
 * @brief   ゲームエンジンの初期化を担当する
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief ゲームエンジンの初期化を担当する
	 */
	class EngineInitializer
	{
		/** @brief 初期化済みフラグ */
		inline static bool m_isInitialized = false;

	public:

		/// @name ライフサイクル
		/// @{

		EngineInitializer() = default;

		~EngineInitializer() = default;

		/// @}

		/**
		 * @brief エンジンの初期化を行う
		 */
		static void EngineInitialize();

	private:

		/**
		 * @brief 衝突マップに3D衝突関数の登録をする
		 */
		static void Regist3DCollisions();

		/**
		 * @brief 衝突マップに2D衝突関数の登録をする
		 */
		static void Regist2DCollisions();

		/**
		 * @brief Rayと3Dコライダーの衝突関数を登録する
		 */
		static void RegistRayCollisions();

		/**
		 * @brief 入力システムにカスタムキーを登録する
		 */
		static void RegistCustomKeys();
	};
} // namespace RHEngine
