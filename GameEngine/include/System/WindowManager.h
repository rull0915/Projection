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

namespace REngine
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

		int m_width;
		int m_height;

		// 背景色
		DirectX::SimpleMath::Color m_backGroundColor;

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
	private:
		WindowManager()
			: m_width{ 1280 }
			, m_height{ 720 }
			, m_backGroundColor{ 0.3f, 0.6f, 0.8f, 1.0f }
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

		// フルスクリーンとウィンドウを切り替える関数
		void SwitchScreenMode(HWND hWnd, Game* game);

		// -1 ~ +1 の座標系からピクセル座標に変換する関数
		DirectX::SimpleMath::Vector2 PixelToScreen(DirectX::SimpleMath::Vector2 pos)
		{
			float xRatio = (pos.x + 1) / 2;
			float yRatio = (pos.y + 1) / 2;

			return { m_width * xRatio, m_height * yRatio };
		}
		// ピクセル座標から -1 ~ +1 の座標系に変換する関数
		DirectX::SimpleMath::Vector2 ScreenToPixel(DirectX::SimpleMath::Vector2 pos)
		{
			float xRatio = pos.x / m_width;
			float yRatio = pos.y / m_height;

			return { xRatio * 2 - 1, -(yRatio * 2 - 1) };
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		int GetWidth() const { return m_width; }    // 幅
		int GetHeight() const { return m_height; }  // 高さ

		float GetWidthF() const { return static_cast<float>(m_width); }    // 幅
		float GetHeightF() const { return static_cast<float>(m_height); }  // 高さ

		DirectX::SimpleMath::Vector2 GetSize() { return { GetWidthF(), GetHeightF() }; }    // サイズ

		float GetAspect() const { return GetWidthF() / GetHeightF(); }	// アスペクト比

		DirectX::SimpleMath::Color GetBackGroundColor() const { return m_backGroundColor; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetWidth(int value) { m_width = value; }    // 幅
		void SetHeight(int value) { m_height = value; }  // 高さ

		void SetBackGroundColor(DirectX::SimpleMath::Color color) { m_backGroundColor = color; }	// 色
	};
}	// namespace REngine
