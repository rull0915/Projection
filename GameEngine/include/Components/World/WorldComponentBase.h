//====================================================//
// ファイル名   : WorldComponentBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/28
//
// 概要 : World空間のコンポーネントの基底クラス
//
// 更新履歴 :
// 2026/06/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/ComponentBase.h"
#include "Components/ComponentInfo.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class WorldComponentBase : public ComponentBase
	{
	public:
		// 自身の所属空間
		static constexpr ComponentSpace SPACE = ComponentSpace::World;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		WorldComponentBase(IComponentOwner* owner)
			: ComponentBase(owner)
		{}

		virtual ~WorldComponentBase() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(WorldComponentBase, ComponentBase)
	};
} // namespace REngine
