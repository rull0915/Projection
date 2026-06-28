//====================================================//
// ファイル名   : ComponentCategory.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/26
//
// 概要 : コンポーネントのカテゴリを宣言した列挙型
//
// 更新履歴 :
// 2026/06/26 新規作成
// 2026/06/28 ComponentSpaceの追加
//====================================================//

#pragma once

//====================================================//
// 列挙型宣言
//====================================================//

// コンポーネントが所属する空間
enum class ComponentSpace : unsigned char
{
	World,
	UI,
};

// コンポーネントのカテゴリ
enum class ComponentCategory : unsigned char
{
	// WorldComponent
	Original,
	Camera,
	Collider,
	Collider2D,
	Renderer,

	// UIComponent
	UIOriginal,
	UIGraphic
};
