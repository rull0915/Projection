//====================================================//
// ファイル名   : AudioListener.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : オーディオを受け取るコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <Audio.h>
#include <cstdint>

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "Components/World/Transform/Transform.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AudioListener : public WorldComponentBase
	{
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// トランスフォームポインタ
		Transform* m_pTransform;

		// リスナー
		std::unique_ptr<DirectX::AudioListener> m_listener;

		// バージョン
		uint32_t m_nowVersion;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AudioListener(IComponentOwner* own);
		~AudioListener() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Awake() override;

		void LateUpdate(const GameTimer& timer) override;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<AudioListener>();
		}

		// リスナー
		DirectX::AudioListener* GetListener() const
		{
			return m_listener.get();
		}
	};
} // namespace REngine
