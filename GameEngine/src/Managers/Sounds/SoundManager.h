//====================================================//
// ファイル名   : SoundManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/01
//
// 概要 : 音系コンポーネント管理クラス
//
// 更新履歴 :
// 2026/07/01 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "../ReserveContainer.h"
#include "Components/Both/Sounds/AudioSource.h"
#include "Components/World/Sounds/AudioListener.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class SoundManager : public ReserveContainer<AudioSource>
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 使用中のリスナー
		AudioListener* m_listener;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		SoundManager();
		~SoundManager() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 更新処理
		void Update();

		// リスナーの設定
		void SetListener(AudioListener* l)
		{
			// 2人目は登録不可
			if (m_listener) return;

			m_listener = l;
		}

		// リスナーの削除
		void RemoveListener(AudioListener* l) { if (m_listener == l) m_listener = nullptr; }
	};
}	// namespace REngine
