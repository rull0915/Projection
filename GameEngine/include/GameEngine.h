/**
 * @file    GameEngine.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/16
 *
 * @brief   RHEngineを統括するクラス
 */

#pragma once

#include <memory>
#include <string>
#include <Windows.h>

#include "Renderer/Renderer.h"
#include "Timer/GameTimer.h"
#include "System/DeviceResources.h"

#include "Assets/Managers/AssetManager.h"
#include "Renderer/Shadow/ShadowRenderer.h"

#ifdef ENGINE_GUI

#include "Editor/SceneEditor.h"

#endif // ENGINE_GUI

namespace RHEngine
{
	/**
	 * @brief RHEngineを統括する
	 */
	class GameEngine
	{
	private:

		/** @brief エンジンのモードが変わるかどうか */
		bool m_modeChange;

		/** @brief タイマー */
		std::unique_ptr<GameTimer> m_gameTimer;

		/** @brief アセット管理者 */
		AssetManager m_assetManager;

		/** @brief 描画担当*/
		Renderer m_renderer;

		/** @brief 影描画担当*/
		ShadowRenderer m_shadowRenderer;

#ifdef ENGINE_GUI

		/** @brief シーンエディタが起動しているかどうか */
		bool m_sceneEdit;

		/** @brief シーンエディタの本体 */
		std::unique_ptr<SceneEditor> m_editor;

#endif // ENGINE_GUI

	public:

		/// @namespace RHEngine
		/// @{

		GameEngine();

		~GameEngine() = default;

		/// @}

		/**
		 * @brief エンジン全体の初期化を行う
		 * @param deviceResources デバイスリソース
		 * @param window ウィンドウハンドル
		 * @param edit 開始時にシーンエディタを起動するかどうか
		 */
		void Initialize(DX::DeviceResources* deviceResources, HWND window, bool edit);

		/**
		 * @brief フレーム開始時の処理
		 */
		void BeginFrame();

		/**
		 * @brief エンジン全体の更新をする
		 * @param elapsedTime 1fの経過時間
		 */
		void Update(float elapsedTime);

		/**
		 * @brief ウィンドウサイズの変化をシーンマネージャーに通知する 
		 */
		void OnWindowSizeChanged(int width, int height);

		/**
		 * @brief エンジン全体の描画をする
		 */
		void Render();

		/**
		 * @brief 終了時にシーンを解放する
		 */
		void Finalize();

#ifdef ENGINE_GUI

		/**
		 * @brief シーンエディタを開始する
		 */
		void StartEditor();

		/**
		 * @brief シーンエディタを終了する
		 * @param initSceneName 終了後に読み込むシーン名
		 */
		void EndEditor(std::string initSceneName);

#endif // ENGINE_GUI

		/// @name ゲッター
		/// @{

		/** @brief タイマー */
		GameTimer& GetTimer() { return *m_gameTimer; }

		/** @brief 描画担当 */
		Renderer& GetRenderer() { return m_renderer; }

		/// @}
	};
}	// namespace RHEngine
