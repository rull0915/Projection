/**
 * @file    ComponentInfo.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/26
 *
 * @brief   コンポーネントの情報をまとめたヘッダ
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief コンポーネントの所属プロジェクト
	 */
	enum class ComponentProject : unsigned char
	{
		Engine,	///< ゲームエンジンデフォルトのコンポーネント
		Game,	///< ゲーム独自のコンポーネント
	};

	/**
	 * @brief コンポーネントが所属する空間
	 */
	enum class ComponentSpace : unsigned char
	{
		None,
		World,
		UI,
		Both,
	};

	/**
	 * @brief コンポーネントの情報
	 */
	struct ComponentInfo
	{
		ComponentProject project;
		ComponentSpace space;
	};
} // namespace RHEngine
