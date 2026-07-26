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
#include "Editor/SceneEditor.h"
#include "System/DeviceResources.h"

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

		// ゲームタイマー
		std::unique_ptr<GameTimer> m_gameTimer;

		// 描画担当
		std::unique_ptr<Renderer> m_renderer;

		// アセット管理
		std::unique_ptr<AssetManager> m_assetManager;

		// エディター
		std::unique_ptr<SceneEditor> m_editor;

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
		void Initialize(DX::DeviceResources* deviceResources, HWND window);

		// 更新関数
		void BeginFrame();

		// 更新関数
		void Update(float elapsedTime);

		// 描画関数
		void Render();

		// 終了関数
		void Finalize();

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		GameTimer& GetTimer() { return *m_gameTimer; }
		Renderer& GetRenderer() { return *m_renderer; }

	private:

		// アセットの登録関数
		void RegistryAssets();
	};
}	// namespace REngine
