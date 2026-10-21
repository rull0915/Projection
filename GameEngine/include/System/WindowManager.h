//====================================================//
// ファイル名   : WindowManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/10
//
// 概要 : ウィンドウの状態を管理するクラスです
//
// 更新履歴 :
// 2026/06/10 新規作成
//====================================================//

#pragma once
#include "Windows.h"

//====================================================//
// インクルードファイル
//====================================================//

//====================================================//
// マクロ宣言
//====================================================//

// ウインドウスタイル
#define WS_MYSTYLE ( WS_OVERLAPPED     | \
					 WS_CAPTION        | \
					 WS_SYSMENU        | \
					 WS_MINIMIZEBOX )

namespace RHEngine
{

//====================================================//
// クラス宣言
//====================================================//
	class WindowManager
	{
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------
	private:

		// デフォルトサイズ
		unsigned int m_defaultWidth;
		unsigned int m_defaultHeight;

		// サイズ
		unsigned int m_width;
		unsigned int m_height;

		// 背景色
		DirectX::SimpleMath::Color m_backGroundColor;

		// ウィンドウ
		HWND m_window;

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
	private:
		WindowManager()
			: m_defaultWidth{}
			, m_defaultHeight{}
			, m_width{}
			, m_height{}
			, m_backGroundColor{ 0.3f, 0.6f, 0.8f, 1.0f }
			, m_window{}
		{};
		~WindowManager() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
	public:

		// シングルトン化
		static WindowManager& Instance()
		{
			static WindowManager instance;
			return instance;
		}

		// デフォルトウィンドウサイズの設定
		void SetDefaultWindowSize(int width, int height)
		{
			m_defaultWidth = static_cast<unsigned int>(width);
			m_defaultHeight = static_cast<unsigned int>(height);
		}

		// ウィンドウをセットする関数
		void SetWindow(HWND hwnd) { m_window = hwnd; }

		// フルスクリーンとウィンドウを切り替える関数
		void SwitchScreenMode();
		
		// NDCの座標系からピクセル座標に変換する関数
		DirectX::SimpleMath::Vector2 PixelToNDC(DirectX::SimpleMath::Vector2 pos)
		{
			float xRatio = (pos.x + 1) / 2;
			float yRatio = (pos.y + 1) / 2;

			return { m_width * xRatio, m_height * yRatio };
		}
		// ピクセル座標からNDCの座標系に変換する関数
		DirectX::SimpleMath::Vector2 NDCToPixel(DirectX::SimpleMath::Vector2 pos)
		{
			float xRatio = pos.x / m_width;
			float yRatio = pos.y / m_height;

			return { xRatio * 2 - 1, -(yRatio * 2 - 1) };
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		unsigned int GetWidth() const { return m_width; }    // 幅
		unsigned int GetHeight() const { return m_height; }  // 高さ

		float GetWidthF() const { return static_cast<float>(m_width); }    // 幅
		float GetHeightF() const { return static_cast<float>(m_height); }  // 高さ

		DirectX::SimpleMath::Vector2 GetSize() { return { GetWidthF(), GetHeightF() }; }   // サイズ
		float GetAspect() const { return GetWidthF() / GetHeightF(); }	// アスペクト比

		// 背景色
		DirectX::SimpleMath::Color GetBackGroundColor() const { return m_backGroundColor; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetWidth(unsigned int value) { m_width = value; }    // 幅
		void SetHeight(unsigned int value) { m_height = value; }  // 高さ
		void SetSize(unsigned int width, unsigned int height) 
		{ 
			m_width = width; 
			m_height = height; 
		} 

		void SetBackGroundColor(DirectX::SimpleMath::Color color) { m_backGroundColor = color; }	// 色
	};
}	// namespace RHEngine
