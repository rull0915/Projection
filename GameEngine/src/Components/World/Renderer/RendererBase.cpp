/**
 * @file    RendererBase.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/02
 */

#include "pch.h"
#include "Components/World/Renderer/RendererBase.h"

#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	RendererBase::RendererBase(IComponentOwner* pOwner)
		: WorldComponentBase(pOwner)
		, m_pTransform{ GetComponent<Transform>() }
	{
	};
}	// namespace RHEngine
