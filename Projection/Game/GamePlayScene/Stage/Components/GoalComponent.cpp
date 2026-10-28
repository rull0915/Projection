/**
 * @file    GoalComponent.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/30
 */

#include "pch.h"
#include "GoalComponent.h"

#include "Physics/HitContact.h"
#include "Components/World/Collider/3D/ColliderBase.h"

#include "Common/EventBus.h"
#include "Game/GamePlayScene/GamePlayEvent.h"

GoalComponent::GoalComponent(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
{
}

// トリガーで衝突したら
void GoalComponent::OnTriggerEnter(RHEngine::HitContact & contact)
{
	// プレイヤーとぶつかったら
	if (contact.other->GetTag() == "Player")
	{
		// ゴールイベントの呼び出し
		RHEngine::EventBus<GamePlayEvent>::Publish(GamePlayEvent::Goal);

		// コライダーの無効化
		contact.ownCol->SetActive(false);
	}
}
