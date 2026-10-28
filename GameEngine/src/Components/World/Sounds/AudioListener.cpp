/**
 * @file    AudioListener.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/01
 */

#include "pch.h"
#include "Components/World/Sounds/AudioListener.h"

#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	AudioListener::AudioListener(IComponentOwner* pOwner)
		: WorldComponentBase(pOwner)
		, m_pTransform{ nullptr }
		, m_listener{}
		, m_nowVersion{ 0 }
	{}

	void AudioListener::Awake()
	{
		// トランスフォームのセット
		m_pTransform = GetComponent<Transform>();

		// リスナーの初期化
		m_listener = std::make_unique<DirectX::AudioListener>();

		// 初期設定

		// 位置
		m_listener->SetPosition(m_pTransform->GetWorldPosition());

		// 向き
		m_listener->SetOrientation(
			m_pTransform->GetForward(), m_pTransform->GetRight()
		);
	}

	// 遅延更新
	void AudioListener::LateUpdate(const GameTimer& timer)
	{
		// 位置の変更後に実行するため遅延更新

		// バージョンを比較
		uint32_t version = m_pTransform->GetVersion();

		// 更新されていたら
		if (m_nowVersion != version)
		{
			// リスナーの情報を更新
			// 位置
			m_listener->SetPosition(m_pTransform->GetWorldPosition());

			// 向き
			m_listener->SetOrientation(
				m_pTransform->GetForward(), m_pTransform->GetUp()
			);

			// バージョンの適用
			m_nowVersion = version;
		}
	}
}	// namespace RHEngine
