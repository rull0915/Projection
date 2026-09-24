//====================================================//
// ファイル名   : GameEngine.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/16
//
// 概要 : Engine部分を統括するクラス
//
// 更新履歴 :
// 2026/07/16 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <memory>

#include "Renderer/Renderer.h"
#include "Assets/Managers/AssetManager.h"
#include "Timer/GameTimer.h"
#include "System/DeviceResources.h"

#ifdef ENGINE_GUI

#include "Editor/SceneEditor.h"

#endif // ENGINE_GUI

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class GameEngine
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// モード変更フラグ
		bool m_modeChange;

		// ゲームタイマー
		std::unique_ptr<GameTimer> m_gameTimer;

		// 描画担当
		std::unique_ptr<Renderer> m_renderer;

		// アセット管理
		std::unique_ptr<AssetManager> m_assetManager;

#ifdef ENGINE_GUI

		// シーンエディットフラグ
		bool m_sceneEdit;

		// エディター
		std::unique_ptr<SceneEditor> m_editor;

#endif // ENGINE_GUI

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		GameEngine();
		~GameEngine() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize(DX::DeviceResources* deviceResources, HWND window, bool edit);

		// 更新関数
		void BeginFrame();

		// 更新関数
		void Update(float elapsedTime);

		// 描画関数
		void Render();

		// 終了関数
		void Finalize();

#ifdef ENGINE_GUI

		// エディタの開始関数
		void StartEditor();

		// エディタの終了関数
		void EndEditor(std::string initSceneName);

#endif // ENGINE_GUI

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		GameTimer& GetTimer() { return *m_gameTimer; }
		Renderer& GetRenderer() { return *m_renderer; }

	};
}	// namespace REngine
