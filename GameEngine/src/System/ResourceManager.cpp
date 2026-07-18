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
#include "Common/GetExtension.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

namespace REngine
{
	ResourceManager::ResourceManager()
		: m_device{ nullptr }
		, m_effect{}
		, m_textures{}
		, m_audioEngine{}
	{}

	ResourceManager::~ResourceManager()
	{}

	void ResourceManager::Initialize()
	{
		auto* deviceResources = GraphicsManager::Instance().GetDeviceResources();

		m_device = deviceResources->GetD3DDevice();

		m_effect = std::make_unique<EffectFactory>(m_device);
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

	void ResourceManager::AddTexture(const std::string& keyName, const std::wstring& filePath)
	{
		// キーの重複チェック
		if (m_textures.find(keyName) != m_textures.end()) return;

		// テクスチャを用意
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		HRESULT hr;

		// キーの拡張子を取得
		std::string extension = GetExtension::Get(filePath);

		if (extension == ".dds")
		{
			hr = CreateDDSTextureFromFile(
				m_device,                 // ID3D11Device*
				filePath.data(),          // ファイルパス
				nullptr,
				texture.GetAddressOf()
			);
		}
		else
		{
			hr = CreateWICTextureFromFile(
				m_device,                 // ID3D11Device*
				filePath.data(),          // ファイルパス
				nullptr,
				texture.GetAddressOf()
			);
		}

		if (FAILED(hr))
		{
		}

		m_textures.insert(std::make_pair(keyName, texture));
	}

	void ResourceManager::AddFont(const std::string& key, const std::wstring& filePath)
	{
		// 既に同じキーが存在する場合はスキップ
		if (m_spriteFonts.find(key) != m_spriteFonts.end()) return;

		// 読み込み
		m_spriteFonts.insert(std::make_pair(key, std::make_unique<SpriteFont>(m_device, filePath.c_str())));

		// Aの文字を描画した時の高さをフォントサイズとする
		SpriteFont* font = m_spriteFonts[key].get();

		// デフォルト文字を「?」に設定
		font->SetDefaultCharacter(L'?');

		float height;

		RECT result = font->MeasureDrawBounds(L"A", XMFLOAT2(0.f, 0.f));
		height = std::fabsf(static_cast<float>(result.bottom - result.top));

		m_spriteSize.insert(std::make_pair(font, height));
	}

	void ResourceManager::AddModel(const std::string& key, const std::wstring& filePath)
	{
		// 既に同じキーが存在する場合はスキップ
		if (m_models.find(key) != m_models.end()) return;

		// キーの拡張子を取得
		std::string extension = GetExtension::Get(filePath);

		std::unique_ptr<DirectX::Model> model;

		// CMOファイルの場合
		if (extension == ".cmo")
		{
			model = Model::CreateFromCMO(m_device, filePath.c_str(), *m_effect);
		}
		// SDKMESHファイルの場合
		else if (extension == ".sdkmesh")
		{
			model = Model::CreateFromSDKMESH(m_device, filePath.c_str(), *m_effect);
		}

		m_models.insert(std::make_pair(key, std::move(model)));
	}

	void ResourceManager::AddSound(const std::string& key, const std::wstring& filePath)
	{
		// 既に同じキーが存在する場合はスキップ
		if (m_models.find(key) != m_models.end()) return;

		auto sound = std::make_unique<DirectX::SoundEffect>(m_audioEngine.get(), filePath.c_str());

		m_sounds.insert(std::make_pair(key, std::move(sound)));
	}

	ID3D11ShaderResourceView* ResourceManager::GetTexture(const std::string& key) const
	{
		auto it = m_textures.find(key);
		if (it == m_textures.end()) return nullptr;
		return it->second.Get();
	}

	SpriteFont* ResourceManager::GetSpriteFont(const std::string& key) const
	{
		// 存在しないキーなら
		auto it = m_spriteFonts.find(key);
		if (it == m_spriteFonts.end()) return nullptr;
		return it->second.get();
	}

	float ResourceManager::GetFontSize(DirectX::SpriteFont* key) const
	{
		// 存在しないキーなら
		auto it = m_spriteSize.find(key);
		if (it == m_spriteSize.end()) return -1;
		return it->second;
	}

	DirectX::Model* ResourceManager::GetModel(const std::string& key) const
	{
		// 存在しないキーなら
		auto it = m_models.find(key);
		if (it == m_models.end()) return nullptr;
		return it->second.get();
	}

	DirectX::SoundEffect* ResourceManager::GetSound(const std::string& key) const
	{
		// 存在しないキーなら
		auto it = m_sounds.find(key);
		if (it == m_sounds.end()) return nullptr;
		return it->second.get();
	}
} // namespace REngine
