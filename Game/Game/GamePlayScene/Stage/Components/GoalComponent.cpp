//====================================================//
// ファイル名  : GoalComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/30
//
// 概要       : ゴールコンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GoalComponent.h"

#include "Physics/HitContact.h"
#include "Components/World/Collider/3D/ColliderBase.h"

#include "Common/EventBus.h"
#include "Game/GamePlayScene/GamePlayEvent.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
GoalComponent::GoalComponent(IComponentOwner* own)
	: WorldComponentBase(own)
{
}

// 生成直後に一度呼ばれます
void GoalComponent::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void GoalComponent::Start()
{}

// 毎フレーム呼ばれます
void GoalComponent::Update(const GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void GoalComponent::LateUpdate(const GameTimer & gameTimer)
{}

// トリガーで衝突したら
void GoalComponent::OnTriggerEnter(HitContact & contact)
{
	// プレイヤーとぶつかったら
	if (contact.other->GetTag() == "Player")
	{
		// ゴールイベントの呼び出し
		EventBus<GamePlayEvent>::Publish(GamePlayEvent::Goal);

		// コライダーの無効化
		contact.ownCol->SetActive(false);
	}
}
