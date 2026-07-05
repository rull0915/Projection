//====================================================//
// ファイル名   : ButtonUI.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : ボタンUIクラス
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "UIBehaviorBase.h"
#include <functional>

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ButtonUI : public UIBehaviorBase
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 各状態の色

	DirectX::SimpleMath::Color m_normalColor;  // 通常時
	DirectX::SimpleMath::Color m_hoverdColor;  // ホバー時
	DirectX::SimpleMath::Color m_pressedColor; // 押されている時

	// クリック時に実行される処理
	std::function<void()> m_onClick;

	bool m_isPressed;   // 自分が押されているか

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ButtonUI(IComponentOwner* own);
	~ButtonUI();

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------
	void Update(const GameTimer& gameTimer) override;

	// マウスクリック
	void OnMouseDown() override;
	void OnMouseUp() override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<ButtonUI>();
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	void SetNormalColor(DirectX::SimpleMath::Color color) { m_normalColor = color; }
	void SetHoveredColor(DirectX::SimpleMath::Color color) { m_hoverdColor = color; }
	void SetPressedColor(DirectX::SimpleMath::Color color) { m_pressedColor = color; }

	void SetOnClick(std::function<void()> func) { m_onClick = func; }

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

};
