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

//====================================================//
// クラス宣言
//====================================================//
class UIComponentBase : public ComponentBase
{
public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	UIComponentBase(IComponentOwner* owner)
		: ComponentBase(owner)
	{
	}

	virtual ~UIComponentBase() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 所属空間をUIに設定
	ComponentSpace GetSpace() const override
	{
		return ComponentSpace::UI;
	}

	// カテゴリをUIに指定
	ComponentCategory GetCategory() const override
	{
		return Category::UIOriginal;
	}
};