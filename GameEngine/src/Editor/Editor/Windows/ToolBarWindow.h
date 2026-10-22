//====================================================//
// ファイル名   : ToolBarWindow.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/21
//
// 概要 : GUIのツールバーを表示するクラス
//
// 更新履歴 :
// 2026/10/21 新規作成
//====================================================//

#pragma once

#ifdef ENGINE_GUI

//====================================================//
// インクルードファイル
//====================================================//

#include <functional>

namespace RHEngine
{
	class Scene;
	class AssetManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class ToolBarWindow
	{
	public:
		// 表示結果

		enum class Result
		{
			None,
			PressLoadButton, // ロードボタン押下
		};

	public:

		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------

		static constexpr float TOOL_BAR_HEIGHT = 30.0f;

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		std::function<void()> m_playFunc;	// プレイ関数

		std::function<void()> m_stopFunc;	// ストップ関数

		Scene* m_pScene;	// シーンポインタ

		AssetManager& m_assetManager;	// アセットマネージャー

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ToolBarWindow(Scene* pScene, AssetManager& assetManager, std::function<void()> play, std::function<void()> stop)
			: m_playFunc{ play }
			, m_stopFunc{ stop }
			, m_pScene{ pScene }
			, m_assetManager{ assetManager }
		{
		}
		~ToolBarWindow() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// ツールバーを表示する関数
		Result DrawToolbar(ID3D11ShaderResourceView* engineLogo, bool isPlaying);

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// ツールバーウィンドウを開始する関数
		void StartToolBarWindow();

		// ツールバーウィンドウを表示する関数
		Result DrawToolBarWindow(ID3D11ShaderResourceView* engineLogo, bool isPlaying);

		// 保存関数
		void Save();

		// ロード関数
		void Load();
	};
}

#endif
