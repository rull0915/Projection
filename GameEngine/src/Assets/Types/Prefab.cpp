/**
 * @file    Prefab.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/28
 */

#include "pch.h"
#include "Assets/Types/Prefab.h"

#include "Assets/Types/PrefabImpl.h"

namespace RHEngine
{
	Prefab::Prefab(std::filesystem::path path)
		: m_impl{ std::make_unique<PrefabImpl>(path) }
	{}

	Prefab::Prefab(std::unique_ptr<PrefabImpl> impl)
		: m_impl{ std::move(impl) }
	{}
}	// namespace RHEngine
