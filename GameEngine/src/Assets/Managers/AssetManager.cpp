//====================================================//
// ファイル名  : AssetManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Managers/AssetManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void REngine::AssetManager::Initialize(const std::wstring& root)
{
	// スキャン
	m_dataBase.ScanFile(root, m_typeManager);
}

void REngine::AssetManager::Update()
{
	// Jobがなければ何もしない
	if (m_asyncJobs.empty()) return;

	// ロードが終わったJobを削除する
	std::erase_if(m_asyncJobs,
		[this](AsyncJob& job)
		{
			// 終了していなければfalse
			if (job.future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
				return false;

			// 終了していれば

			// 生成したAssetを取得
			auto asset = job.future.get();

			// Assetのステータスを読み込み済みに変更
			asset->SetStatus(LoadStatus::Loaded);

			// 置き換える
			m_registry.Replace(job.index, std::move(asset));

			// 削除してもらう
			return true;
		}
	);
}
