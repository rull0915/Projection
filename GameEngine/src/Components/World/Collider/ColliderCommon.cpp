/**
 * @file    ColliderCommon.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/22
 */

#include "pch.h"
#include "Components/World/Collider/ColliderCommon.h"

#include "Assets/Managers/AssetManager.h"

namespace RHEngine
{
	ColliderCommon::ColliderCommon(IComponentOwner* pOwner)
		: WorldComponentBase(pOwner)
		, m_pTransform{ GetComponent<Transform>() }
		, m_isTrigger{ false }
		, m_isDirty{ true }
		, m_isChanged{ false }
		, m_layerNum{ 0 }
		, m_latestVersion{ 1 }
		, m_physicsMaterial{}
		, m_colliderReceiver{ dynamic_cast<IColliderReceiver*>(pOwner) }
		, m_needInfo{ false }
		, m_pAssetManager{ nullptr }
	{
		ADD_PROPERTY(ColliderCommon, m_isTrigger);
		ADD_PROPERTY(ColliderCommon, m_needInfo);
		ADD_PROPERTY(ColliderCommon, m_physicsMaterial);
		ADD_PROPERTY(ColliderCommon, m_layerNum);
	}

	const PhysicsMaterial* ColliderCommon::GetPhysicsMaterial() const
	{
		return m_pAssetManager->Get<PhysicsMaterial>(m_physicsMaterial);
	}
}	// namespace RHEngine
