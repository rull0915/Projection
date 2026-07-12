//====================================================//
// ファイル名   : MainScreen.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/12
//
// 概要 : メインスクリーンクラス
//
// 更新履歴 :
// 2026/07/12 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "System/Render/RenderTarget.h"

class Renderer;
//====================================================//
// クラス宣言
//====================================================//

/// <summary>
/// メインスクリーン
/// </summary>
class MainScreen
{
private:
    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------
	
	// 描画するかどうか
	bool m_draw;

	// 使用する描画ターゲット
	std::unique_ptr<RenderTarget> m_defaultRenderTarget;

	// 描画の始点
	DirectX::SimpleMath::Vector2 m_startPoint;

	// 描画の拡大率
	DirectX::SimpleMath::Vector2 m_scale;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    MainScreen();
    ~MainScreen() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------
	
	// 描画関数
	void Render(Renderer& renderer);

	// メインスクリーンの位置を反映したマウス位置を取得する関数
	DirectX::SimpleMath::Vector2 GetMousePointOnMainScreen();

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

	// 始点
	DirectX::SimpleMath::Vector2 GetMainScreenStartPoint() const { return m_startPoint; }

	// 拡大率
	DirectX::SimpleMath::Vector2 GetMainScreenScale() const { return m_scale; }

	// レンダーターゲット
	RenderTarget* GetRenderTarget() const { return m_defaultRenderTarget.get(); }

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

	// 始点
	void SetMainScreenStartPoint(DirectX::SimpleMath::Vector2 p) { m_startPoint = p; };

	// 拡大率
	void SetMainScreenScale(DirectX::SimpleMath::Vector2 p) { m_scale = p; };

	// 描画フラグ
	void SetIsDraw(bool f) { m_draw = f; }

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

};
