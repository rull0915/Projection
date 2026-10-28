/**
 * @file    SoundManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/07/01
 */

#include "pch.h"
#include "SoundManager.h"

#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	// コンストラクタ
	SoundManager::SoundManager()
		: m_listener{ nullptr }
	{}

	// 更新関数
	void SoundManager::Update()
	{
		// リスナーがなければ何もしない
		if (!m_listener) return;

		// リスナーを取得
		DirectX::AudioListener* listener = m_listener->GetListener();

		// Emitterを生成
		DirectX::AudioEmitter emitter;

		// 全ソースをループ
		for (auto& source : GetObjects())
		{
			// 3Dソースなら
			if (source->Is3D())
			{
				// Transformを取得
				if (Transform* transform = source->GetComponent<Transform>())
				{
					// Emitterを設定
					emitter.SetPosition(transform->GetWorldPosition());

					emitter.SetOrientation(
						transform->GetForward(), transform->GetUp()
					);

					// 3Dを適用
					if (const auto& instance = source->GetSoundInstance())
					{
						instance->Apply3D(
							*listener, emitter
						);
					}
				}
			}
		}
	}
}	// namespace RHEngine
