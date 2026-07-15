//====================================================//
// ファイル名  : ColliderCommon.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/22
//
// 概要        : Colliderインターフェース
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"
#include "Components/World/Collider/ColliderCommon.h"

namespace REngine
{
	ColliderCommon::ColliderCommon(IComponentOwner* own)
		: WorldComponentBase(own)
		, m_pTransform{ GetComponent<Transform>() }
		, m_isTrigger{ false }
		, m_isDirty{ true }
		, m_isChanged{ false }
		, m_layerNum{ 0 }
		, m_latestVersion{ 1 }
		, m_physicsMaterial{ nullptr }
		, m_colliderReceiver{ dynamic_cast<IColliderReceiver*>(own) }
		, m_needInfo{ false }
	{
		ADD_PROPERTY(m_isTrigger);
		ADD_PROPERTY(m_needInfo);
		ADD_PROPERTY(m_layerNum);
	}
}	// namespace REngine
