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

// コンポーネントの所属プロジェクト
enum class ComponentProject : unsigned char
{
	Engine,
	Game,
};

// コンポーネントが所属する空間
enum class ComponentSpace : unsigned char
{
	None,
	World,
	UI,
};

// コンポーネントの情報をまとめた構造体
struct ComponentInfo
{
	ComponentProject project;
	ComponentSpace space;
};

using ComponentCategory = unsigned char;

// コンポーネントのカテゴリ
namespace Category
{
	// WorldComponent
	static constexpr ComponentCategory Original		= 0;
	static constexpr ComponentCategory Camera		= 1;
	static constexpr ComponentCategory Collider		= 2;
	static constexpr ComponentCategory Collider2D	= 3;
	static constexpr ComponentCategory Renderer		= 4;

	// UIComponent
	static constexpr ComponentCategory UIOriginal	= 5;
	static constexpr ComponentCategory UIGraphic	= 6;
	static constexpr ComponentCategory UIBehavior	= 7;
};
