//====================================================//
// ファイル名  : SoundManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/01
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "SoundManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
SoundManager::SoundManager()
	: m_reserves{}
	, m_removeReserves{}
	, m_sources{}
	, m_listener{ nullptr }
{
}

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
	for (auto& source : m_sources)
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
