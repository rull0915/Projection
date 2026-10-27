/**
 * @file    SceneEditor.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/05
 *
 * @brief   シーンエディタ
 */

#pragma once

#ifdef ENGINE_GUI

#include <memory>

#include "Scene/Scene.h"
#include "Editor/Editor/EditGUI.h"
#include "Editor/Editor/SceneCamera.h"
#include "System/Render/RenderTarget.h"
#include "Mouse.h"

namespace RHEngine
{
	class AssetManager;
	class Renderer;
	class GameTimer;

	/**
	 * @brief シーンエディタ
	 */
	class SceneEditor
	{
	private:

		/** @brief 編集対象のシーン */
		Scene* m_pScene;

		/** @brief エディットが実行中かどうか */
		bool m_isActive;

		/** @brief 再生中かどうか */
		bool m_isPlaying;

		/** @brief GUI本体 */
		EditGUI m_gui;

		/** @brief シーンビューのテクスチャ */
		std::unique_ptr<RenderTarget> m_sceneView;

		/** @brief シーンビューカメラ */
		SceneCamera m_sceneViewCamera;

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

		/**
		 * @brief 以前のマウスモード
		 * @details esc押下時にマウスを絶対モードへ変更する。
		 *			ゲームビュークリック時に元に戻すために使用
		 */
		DirectX::Mouse::Mode m_oldMouseMode;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene 編集対象のシーン
		 * @param assetManager アセットマネージャー
		 */
		SceneEditor(Scene* pScene, AssetManager& assetManager);

		~SceneEditor();

		/// @}

		/**
		 * @brief GUIの初期化 
		 */
		void Initialize();

		/**
		 * @brief GUIの終了
		 */
		void Finalize();

		/**
		 * @brief 更新関数
		 * @details ウィンドウの表示、シーンカメラの移動を行う
		 */
		void Update(const GameTimer& gameTimer);

		/**
		 * @brief シーンビューの描画を行う
		 */
		void Render(Renderer& renderer);

	private:

		/**
		 * @brief テストプレイを開始する
		 */
		void TestPlay();

		/**
		 * @brief テストプレイを終了する
		 */
		void PlayStop();
	};
} // namespace RHEngine

#endif // ENGINE_GUI
