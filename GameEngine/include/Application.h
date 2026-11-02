/**
 * @file    Application.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/14
 *
 * @brief   アプリクラス
 */

#pragma once

#include <string>
#include <cstdint>
#include <memory> 
#include <Windows.h>

#include "System/DeviceResources.h"
#include "Timer/StepTimer.h"

namespace RHEngine
{
	class GameEngine;

	/**
	 * @brief アプリケーションの設定項目
	 */
	struct ApplicationProps {
		std::wstring title = L"RHEngine";	///< アプリタイトル
		uint32_t width = 1280;				///< ウィンドウ幅
		uint32_t height = 720;				///< ウィンドウ高さ
	};

	/**
	 * @brief アプリケーションクラス
	 * @details ゲームプロジェクト側で継承してアプリインスタンスを作成する
	 */
	class Application : public DX::IDeviceNotify 
	{
	private:

		/** @brief アプリの設定 */
		ApplicationProps m_props;

		/** @brief ゲームエンジン本体 */
		std::unique_ptr<GameEngine> m_gameEngine;

		/** @brief デバイスリソース */
		DX::DeviceResources* m_deviceResources = nullptr;

		/** @brief タイマー */
		DX::StepTimer m_timer;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param props アプリの設定
		 */
		Application(const ApplicationProps& props);

		virtual ~Application();

		/// @}

		/// @name ゲーム側がオーバーライドするイベント

		/** @brief 初期化時に呼ばれる */
		virtual void OnInitialize() {}

		/** @brief 終了時に呼ばれる */
		virtual void OnFinalize() {}

		/** @brief 毎フレーム呼ばれる */
		virtual void OnUpdate(float deltaTime) { deltaTime; }

		/// @}

		/**
		 * @brief ゲームエンジンの初期化、入力の初期化などを行う
		 * @param hwnd ウィンドウハンドル
		 * @param width ウィンドウ幅
		 * @param height ウィンドウ高さ
		 */
		void Initialize(HWND hwnd, int width, int height);

		/**
		 * @brief 終了時にゲームエンジンを解放する
		 */
		void Finalize();

		/**
		 * @brief 毎フレームの処理
		 */
		void Tick();

		/**
		 * @brief アプリ情報を取得する
		 */
		const ApplicationProps& GetProps() const { return m_props; }

		/// @name IDeviceNotifyの実装
		/// @{

		/** @brief デバイスロスト時の処理 */
		void OnDeviceLost() override {}

		/** @brief デバイス復帰時の処理 */
		void OnDeviceRestored() override {}

		/// @}

		/// @name ウィンドウプロシージャから呼び出される
		/// @{

		/**
		 * @brief ウィンドウサイズ変更時の処理
		 * @param width 変更後の幅
		 * @param height 変更後の高さ
		 */
		void OnWindowSizeChanged(int width, int height);

		/// @}

	private:

		/**
		 * @brief ゲームエンジン全体を更新し、ゲームの更新処理を呼び出す
		 */
		void Update();
		
		/**
		 * @brief ゲームエンジンを描画する
		 */
		void Render();

		/**
		 * @brief ウィンドウをクリアする
		 */
		void Clear();
	};

	/**
	 * @brief ゲーム側で実装するアプリ作成関数
	 * @details エントリポイントでアプリを作成する際に呼ばれる
	 *			継承した派生クラスを作成する処理をゲームプロジェクト側に書くこと
	 * @return 作成したアプリ
	 */
	Application* CreateApplication();
}	// namespace RHEngine
