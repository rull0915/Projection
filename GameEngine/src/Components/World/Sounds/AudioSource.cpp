//====================================================//
// ファイル名  : AudioSource.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/01
//
// 概要       : オーディオソース
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Sounds/AudioSource.h"

#include "System/ResourceManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
AudioSource::AudioSource(IComponentOwner* own)
	: WorldComponentBase(own)
	, m_soundEffect{}
	, m_soundInstance{ nullptr }
	, m_soundName{}
	, m_volume{ 1.0f }
	, m_pan{ 0.0f }
	, m_pitch{ 0.0f }
	, m_loop{ false }
	, m_playOnStart{ false }
	, m_use3DAudio{ false }
{
	ADD_PROPERTY(m_soundName);
	ADD_PROPERTY(m_volume);
	ADD_PROPERTY(m_pan);
	ADD_PROPERTY(m_pitch);
	ADD_PROPERTY(m_loop);
	ADD_PROPERTY(m_playOnStart);
	ADD_PROPERTY(m_use3DAudio);
}

void AudioSource::Load(const std::string& key, bool use3D)
{
	// 音読み込み
	m_soundEffect = ResourceManager::Instance().GetSound(key);

	m_use3DAudio = use3D;

	// キー名保存
	m_soundName = key;

	// インスタンス作成

	// 3Dを使用するなら
	if (use3D)
	{
		m_soundInstance = m_soundEffect->CreateInstance(DirectX::SoundEffectInstance_Use3D);
	}
	// 使わないなら
	else
	{
		m_soundInstance = m_soundEffect->CreateInstance();
	}
}

// 最初のUpdate関数の直線に一度呼ばれます
void AudioSource::Start()
{
	// 未設定なら
	if (!m_soundInstance && m_soundName.size() > 0)
	{
		Load(m_soundName, m_use3DAudio);
	}

	// 開始時に鳴らすフラグがオンなら
	if (m_playOnStart)
	{
		// インスタンスが生成されていたら
		if (m_soundInstance)
		{
			// 再生
			m_soundInstance->Play(m_loop);
		}
	}
}

void AudioSource::Play() const
{
	// 音が設定されていれば
	if (m_soundInstance)
	{
		// 停止
		m_soundInstance->Stop();

		// 再生
		m_soundInstance->Play(m_loop);
	}
}

void AudioSource::Stop() const
{
}
