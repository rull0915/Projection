/**
 * @file    ResourceManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/03/31
 */

#include "pch.h"
#include "System/ResourceManager.h"

#include "System/Render/GraphicsManager.h"

using namespace DirectX;

namespace RHEngine
{
	ResourceManager::ResourceManager()
		: m_audioEngine{}
	{}

	ResourceManager::~ResourceManager()
	{}

	void ResourceManager::Initialize()
	{
		auto* deviceResources = GraphicsManager::Instance().GetDeviceResources();

		auto device = deviceResources->GetD3DDevice();

		m_audioEngine = std::make_unique<DirectX::AudioEngine>();
	}

	void ResourceManager::Update()
	{
		if (!m_audioEngine->Update())
		{
			// No audio device is active
			if (m_audioEngine->IsCriticalError())
			{
			}
		}
	}
} // namespace RHEngine
