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
#include "Components/Both/Sounds/AudioSource.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
AudioSource::AudioSource(IComponentOwner* own)
	: BothComponentBase(own)
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

// 最初のUpdate関数の直線に一度呼ばれます
void AudioSource::Start()
{ 
	LoadResource();
	ReflectLoading();

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

	// 設定の反映
	ReflectSetting();
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
	m_soundInstance->Stop();
}

void AudioSource::ReflectLoading()
{
	// インスタンス作成
	if (m_soundEffect)
	{
		// 3Dを使用するなら
		if (m_use3DAudio)
		{
			m_soundInstance = m_soundEffect->CreateInstance(DirectX::SoundEffectInstance_Use3D);
		}
		// 使わないなら
		else
		{
			m_soundInstance = m_soundEffect->CreateInstance();
		}
	}
}

void AudioSource::ReflectSetting()
{
	m_volume = std::clamp(m_volume, 0.0f, 1.0f);
	m_pan = std::clamp(m_pan, -1.0f, 1.0f);
	m_pitch = std::clamp(m_pitch, -1.0f, 1.0f);

	if (m_soundInstance) m_soundInstance->SetVolume(m_volume);
	if (m_soundInstance) m_soundInstance->SetPan(m_pan);
	if (m_soundInstance) m_soundInstance->SetPitch(m_pitch);
}
