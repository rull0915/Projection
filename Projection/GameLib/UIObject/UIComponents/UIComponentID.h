//====================================================//
// ファイル名   : UIComponentID.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/11
//
// 概要 : UIコンポーネントのIDを宣言したヘッダ
//
// 更新履歴 :
// 2026/05/11 新規作成
//====================================================//

#pragma once

//====================================================//
// マクロ宣言
//====================================================//

constexpr uint16_t UI_MAIN_MASK = (0xFF00);
constexpr uint16_t UI_SUB_MASK = (0xFFFF);


// コンポーネントを識別するID
enum class UIComponentID
{
	// レクトトランスフォーム
	RectTransform	= 0x0100,

	// グラフィック
	Graphic			= 0x0200,
		Image		= 0x0201,
		Text		= 0x0202,

	// ボタン
	Button			= 0x0300,
};


// MAIN_IDのみを切り取る関数
static constexpr UIComponentID MaskOperation(UIComponentID id, uint16_t mask = UI_MAIN_MASK) 
{
	return static_cast<UIComponentID>(static_cast<int>(id) & mask);
}
