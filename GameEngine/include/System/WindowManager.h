/**
 * @file    WindowManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/10
 *
 * @brief   ウィンドウの状態を管理する
 */

#pragma once

#include <SimpleMath.h>

#include "Windows.h"

// ウインドウスタイル
#define WS_MYSTYLE ( WS_OVERLAPPED     | \
					 WS_CAPTION        | \
					 WS_SYSMENU        | \
					 WS_MINIMIZEBOX )

namespace RHEngine
{
	/**
	 * @brief ウィンドウの状態を管理する
	 */
	class WindowManager
	{
	private:

		/** @brief デフォルトのウィンドウ幅 (フルスクリーンから戻す際に使用) */
		unsigned int m_defaultWidth;

		/** @brief デフォルトのウィンドウ高さ (フルスクリーンから戻す際に使用) */
		unsigned int m_defaultHeight;

		/** @brief 現在のウィンドウ幅 */
		unsigned int m_width;

		/** @brief 現在のウィンドウ高さ */
		unsigned int m_height;

		/** @brief メインウィンドウの背景色 */
		DirectX::SimpleMath::Color m_backGroundColor;

		/** @brief ウィンドウハンドル */
		HWND m_window;

	private:

		/// @name ライフサイクル
		/// @{

		WindowManager()
			: m_defaultWidth{}
			, m_defaultHeight{}
			, m_width{}
			, m_height{}
			, m_backGroundColor{ 0.3f, 0.6f, 0.8f, 1.0f }
			, m_window{}
		{};

		~WindowManager() = default;

		/// @}

	public:

		/**
		 * @brief インスタンスを取得する
		 * @return 静的なインスタンス
		 */
		static WindowManager& Instance()
		{
			static WindowManager instance;
			return instance;
		}

		/**
		 * @brief デフォルトのウィンドウサイズを設定する
		 */
		void SetDefaultWindowSize(int width, int height)
		{
			m_defaultWidth = static_cast<unsigned int>(width);
			m_defaultHeight = static_cast<unsigned int>(height);
		}

		/**
		 * @brief ウィンドウを設定する
		 */
		void SetWindow(HWND hwnd) { m_window = hwnd; }

		/**
		 * @brief フルスクリーンとウィンドウを切り替える
		 */
		void SwitchScreenMode();
		
		/**
		 * @brief ピクセル座標からNDC座標に変換する
		 * @param pos ピクセル座標
		 * @return NDC座標
		 */
		DirectX::SimpleMath::Vector2 PixelToNDC(DirectX::SimpleMath::Vector2 pos)
		{
			float xRatio = (pos.x + 1) / 2;
			float yRatio = (pos.y + 1) / 2;

			return { m_width * xRatio, m_height * yRatio };
		}

		/**
		 * @brief NDC座標系からピクセル座標に変換する
		 * @param pos NDC座標
		 * @return ピクセル座標
		 */
		DirectX::SimpleMath::Vector2 NDCToPixel(DirectX::SimpleMath::Vector2 pos)
		{
			float xRatio = pos.x / m_width;
			float yRatio = pos.y / m_height;

			return { xRatio * 2 - 1, -(yRatio * 2 - 1) };
		}

		/// @name ゲッター
		/// @{

		/** @brief 現在のウィンドウ幅 */
		unsigned int GetWidth() const { return m_width; } 

		/** @brief 現在のウィンドウ高さ */
		unsigned int GetHeight() const { return m_height; }

		/** @brief 現在のウィンドウ幅 (float版) */
		float GetWidthF() const { return static_cast<float>(m_width); }

		/** @brief 現在のウィンドウ高さ (float版) */
		float GetHeightF() const { return static_cast<float>(m_height); }

		/** @brief 現在のウィンドウサイズ */
		DirectX::SimpleMath::Vector2 GetSize() { return { GetWidthF(), GetHeightF() }; }

		/** @brief ウィンドウのアスペクト比 */
		float GetAspect() const { return GetWidthF() / GetHeightF(); }

		/** @brief 背景色 */
		DirectX::SimpleMath::Color GetBackGroundColor() const { return m_backGroundColor; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief ウィンドウ幅 */
		void SetWidth(unsigned int value) { m_width = value; }

		/** @brief ウィンドウ高さ */
		void SetHeight(unsigned int value) { m_height = value; }

		/** @brief ウィンドウサイズ */
		void SetSize(unsigned int width, unsigned int height) 
		{ 
			m_width = width; 
			m_height = height; 
		} 

		/** @brief 背景色 */
		void SetBackGroundColor(DirectX::SimpleMath::Color color) { m_backGroundColor = color; }	// 色
	};
}	// namespace RHEngine
