//====================================================//
// ファイル名   : Application.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/14
//
// 概要 : アプリクラス
//
// 更新履歴 :
// 2026/10/14 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <memory>

#include "System/DeviceResources.h"
#include "Timer/StepTimer.h"

namespace RHEngine
{
	class GameEngine;

	//====================================================//
	// クラス宣言
	//====================================================//

	// アプリケーション設定構造体
	struct ApplicationProps {
		std::wstring title = L"RHEngine";	// アプリタイトル
		uint32_t width = 1280;				// ウィンドウ幅
		uint32_t height = 720;				// ウィンドウ高さ
	};

	// アプリケーションクラス
	class Application : public DX::IDeviceNotify 
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// アプリ情報
		ApplicationProps m_props;

		// ゲームエンジン本体
		std::unique_ptr<GameEngine> m_gameEngine;

		// デバイスリソース
		DX::DeviceResources* m_deviceResources = nullptr;

		// タイマー
		DX::StepTimer m_timer;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Application(const ApplicationProps& props);
		virtual ~Application();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// ゲーム側でがオーバーライドするイベントフック
		virtual void OnInitialize() {}
		virtual void OnFinalize() {}
		virtual void OnUpdate(float deltaTime) { deltaTime; }

		// エンジン内部で使用する関数
		void Initialize(HWND hwnd, int width, int height);
		void Finalize();
		void Tick();

		const ApplicationProps& GetProps() const { return m_props; }

		// IDeviceNotifyの実装
		void OnDeviceLost() override {}
		void OnDeviceRestored() override {}

	private:

		// 更新関数
		void Update();
		
		// 描画関数
		void Render();

		// クリア関数
		void Clear();
	};

	// ゲーム側で実装するファクトリ関数宣言
	Application* CreateApplication();
}
