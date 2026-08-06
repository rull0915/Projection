//====================================================//
// ファイル名  : ResourceManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/03/31
//
// 概要        :  リソース管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "System/ResourceManager.h"
#include "System/GraphicsManager.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	ResourceManager::ResourceManager()
		: m_effect{}
		, m_audioEngine{}
	{}

	ResourceManager::~ResourceManager()
	{}

	void ResourceManager::Initialize()
	{
		auto* deviceResources = GraphicsManager::Instance().GetDeviceResources();

		auto device = deviceResources->GetD3DDevice();

		m_effect = std::make_unique<EffectFactory>(device);
		m_effect->SetDirectory(L"Resources/Models");

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
} // namespace REngine
