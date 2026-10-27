/**
 * @file    ImguiManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/04
 *
 * @brief   Imguiの管理をするクラス
 */

#pragma once

#ifdef ENGINE_GUI

#include <d3d11.h>
#include <Windows.h>

namespace RHEngine
{
	/**
	 * @brief ImGuiの管理をする
	 */
	class ImguiManager
	{
	public:

		/// @name ライフサイクル
		/// @{

		ImguiManager() = default;
		~ImguiManager() = default;

		/// @}

	public:
		/**
		 * @brief ImGuiの初期設定を行う
		 * @param hwnd ウィンドウ
		 * @param device デバイスポインタ　
		 * @param context コンテキストポインタ
		 */
		static void Initialize(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

		/**
		 * @brief ImGuiの更新処理を行う
		 * @details フレーム開始時の処理を行うため、必ず各フレームの最初に呼ぶこと
		 */
		static void Update();

		/**
		 * @brief ImGuiの描画を行う
		 */
		static void Render();

		/**
		 * @brief ImGuiの終了処理を行う
		 */
		static void Finalize();

		/**
		 * @brief ImGuiにウィンドウプロシージャからのメッセージを通知する
		 */
		static bool ProcessMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		/**
		 * @brief ImGuiのスタイルを変更する
		 */
		static void ChangeGUIStyle();
	};
} // namespace RHEngine

#endif // ENGINE_GUI
