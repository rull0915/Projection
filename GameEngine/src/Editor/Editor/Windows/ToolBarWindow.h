/**
 * @file    ToolBarWindow.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/21
 *
 * @brief   GUIのツールバーを表示するクラス
 */

#pragma once

#ifdef ENGINE_GUI

#include <functional>
#include <d3d11.h>

namespace RHEngine
{
	class Scene;
	class AssetManager;

	class ToolBarWindow
	{
	public:

		/**
		 * @brief ツールバーの表示結果
		 */
		enum class Result
		{
			None,
			PressLoadButton, // ロードボタン押下
		};

	public:

		/** @brief ツールバーウィンドウの高さ */
		static constexpr float TOOL_BAR_HEIGHT = 30.0f;

	private:

		/** @brief 再生ボタン押下時の再生関数 */
		std::function<void()> m_playFunc;

		/** @brief 停止ボタン押下時の再生関数 */
		std::function<void()> m_stopFunc;

		/** @brief 編集中のシーン */
		Scene* m_pScene;

		/** @brief アセットマネージャー */
		AssetManager& m_assetManager;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pScene 編集中のシーン
		 * @param assetManager アセットマネージャー
		 * @param play 再生関数
		 * @param stop 停止関数
		 */
		ToolBarWindow(Scene* pScene, AssetManager& assetManager, std::function<void()> play, std::function<void()> stop)
			: m_playFunc{ play }
			, m_stopFunc{ stop }
			, m_pScene{ pScene }
			, m_assetManager{ assetManager }
		{
		}

		~ToolBarWindow() = default;

		/// @}

		/**
		 * @brief ツールバーを表示する
		 * @param pEngineLogo RHEngineのロゴテクスチャ
		 * @param isPlaying 再生中かどうか
		 * @return 表示結果
		 */
		Result DrawToolbar(ID3D11ShaderResourceView* pEngineLogo, bool isPlaying);

	private:

		/**
		 * @brief ツールバーウィンドウの描画を開始する
		 */
		void StartToolBarWindow();

		/**
		 * @brief ツールバーウィンドウを描画する
		 * @param pEngineLogo RHEngineのロゴテクスチャ
		 * @param isPlaying 再生中かどうか
		 * @return 表示結果
		 */
		Result DrawToolBarWindow(ID3D11ShaderResourceView* pEngineLogo, bool isPlaying);

		/**
		 * @brief シーンを保存する
		 */
		void Save();

		/**
		 * @brief シーンをロードする
		 */
		void Load();
	};
}	// namespace RHEngine

#endif	// ENGINE_GUI
