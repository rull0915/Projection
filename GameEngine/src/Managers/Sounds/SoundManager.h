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
#include <unordered_set>

#include "Components/World/Sounds/AudioSource.h"
#include "Components/World/Sounds/AudioListener.h"

//====================================================//
// クラス宣言
//====================================================//
class SoundManager
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 登録予約中のAudioSource
	std::vector<AudioSource*> m_reserves;
	std::unordered_set<AudioSource*> m_removeReserves;

	// 登録されているAudioSource
	std::vector<AudioSource*> m_sources;

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

	// 予約反映
	void ReflectReserves()
	{
		AddReserved();
		RemoveReserved();
	}

	// 登録予約
	void AddAudioSource(AudioSource* r) { m_reserves.push_back(r); }
	void RemoveAudioSource(AudioSource* r) { m_removeReserves.insert(r); }

	// リスナーの設定
	void SetListener(AudioListener* l)
	{
		// 2人目は登録不可
		if (m_listener) return;

		m_listener = l; 
	}

	// リスナーの削除
	void RemoveListener(AudioListener* l) { if (m_listener == l) m_listener = nullptr; }

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	// 予約済みポインタの追加
	void AddReserved()
	{
		for (auto p : m_reserves)
		{
			m_sources.push_back(p);
		}

		m_reserves.clear();
	}

	// 予約済みポインタの削除
	void RemoveReserved()
	{
		// 削除リストに含まれる要素を削除
		std::erase_if(
			m_sources,
			[&](AudioSource* source) ->bool
			{
				return m_removeReserves.contains(source);
			}
		);

		// 削除リストを初期化
		m_removeReserves.clear();
	}
};
