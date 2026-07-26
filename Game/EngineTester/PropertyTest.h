//====================================================//
// ファイル名   : PropertyTest.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/21
//
// 概要 : 
//
// 更新履歴 :
// 2026/07/21 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(PropertyTest)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"

#include "Status.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class PropertyTest : public REngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// ステータス
	Status m_status;

	// id
	float m_id;

	// タイプ
	Type m_type;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	PropertyTest(REngine::IComponentOwner* own);
	~PropertyTest() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;
	void LateUpdate(const REngine::GameTimer& gameTimer) override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return REngine::TypeIDGenerator::GetID<PropertyTest>();
	}
};
