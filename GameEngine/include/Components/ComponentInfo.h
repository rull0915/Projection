//====================================================//
// ファイル名   : ComponentInfo.h
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

namespace REngine
{
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
		Both,
	};

	// コンポーネントの情報をまとめた構造体
	struct ComponentInfo
	{
		ComponentProject project;
		ComponentSpace space;
	};
} // namespace REngine
