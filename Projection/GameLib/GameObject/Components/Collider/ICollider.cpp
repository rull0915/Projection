//====================================================//
// ファイル名  : ICollider.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/22
//
// 概要        : Colliderインターフェース
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"
#include "ICollider.h"

#include "../../GameObject.h"

ICollider::ICollider(IComponentOwner* own, ComponentID id, bool isMain)
	: BaseComponent(own, id, isMain)
	, m_pTransform{ GetComponent<Transform>() }
	, m_isTrigger{ false }
	, m_isDirty{ true }
	, m_isChanged{ false }
	, m_layerNum{ 0 }
	, m_latestVersion{ 1 }
	, m_physicsMaterial{ nullptr }
	, m_colliderReceiver{ dynamic_cast<IColliderReceiver*>(own) }
{
}

void ICollider::SaveIPart(json& js)
{
	js["Layer"] = m_layerNum;
	js["Trigger"] = m_isTrigger;
	js["NeedInfo"] = m_needInfo;
}

void ICollider::LoadIPart(json& js)
{
	m_layerNum = js["Layer"];
	m_isTrigger = js["Trigger"];
	m_needInfo = js["NeedInfo"];
}
