//====================================================//
// ファイル名   : UIComponentBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/11
//
// 概要 : UI用コンポーネント基底クラス
//
// 更新履歴 :
// 2026/05/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/ComponentBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class UIComponentBase : public ComponentBase
	{
	public:
		// 自身の所属空間
		static constexpr ComponentSpace SPACE = ComponentSpace::UI;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		UIComponentBase(IComponentOwner* owner)
			: ComponentBase(owner)
		{}

		virtual ~UIComponentBase() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// カテゴリをUIに指定
		ComponentCategory GetCategory() const override
		{
			return Category::UIOriginal;
		}
	};
} // namespace REngine
