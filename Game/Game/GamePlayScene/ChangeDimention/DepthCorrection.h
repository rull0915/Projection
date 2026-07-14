//====================================================//
// ファイル名   : DepthCorrection.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/24
//
// 概要 : 2Dの際にZ軸の補正を行うコンポーネント
//
// 更新履歴 :
// 2026/06/24 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(DepthCorrection)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class DepthCorrection : public WorldComponentBase
{
private:
	static constexpr float CORRECTION_BORDER = 1.0f / 1.1415926535f;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	DepthCorrection(IComponentOwner* owner)
		: WorldComponentBase(owner)
	{
	}

	~DepthCorrection() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void OnCollisionEnter2D(HitContact2D& contact) override;

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<DepthCorrection>();
	}
};
