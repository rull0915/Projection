//====================================================//
// ファイル名   : BothComponentBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : World,UIどちらにも付けられるコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/ComponentBase.h"
#include "Components/ComponentInfo.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//

namespace REngine
{
	class BothComponentBase : public ComponentBase
	{
	public:
		// 自身の所属空間
		static constexpr ComponentSpace SPACE = ComponentSpace::Both;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		BothComponentBase(IComponentOwner* owner)
			: ComponentBase(owner)
		{}

		virtual ~BothComponentBase() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(BothComponentBase, ComponentBase)
	};
} // namespace REngine
