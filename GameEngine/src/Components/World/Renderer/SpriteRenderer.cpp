/**
 * @file    SpriteRenderer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/10
 */

#include "pch.h"
#include "Components/World/Renderer/SpriteRenderer.h"

#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	SpriteRenderer::SpriteRenderer(IComponentOwner* pOwner)
		: RendererBase(pOwner)
	{
	}

	void SpriteRenderer::Draw(Renderer& renderer)
	{

	};
}	// namespace RHEngine
