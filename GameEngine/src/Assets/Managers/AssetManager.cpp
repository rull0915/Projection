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

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void AssetManager::Initialize(const std::wstring& root)
	{
		// スキャン
		m_dataBase.ScanFile(root, m_typeManager);
	}

	void AssetManager::Update()
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

	UnTypeHandle AssetManager::LoadFromUUID(UUID uuid)
	{
		// 既に読み込まれているUUIDなら
		if (auto it = m_uuidToHandle.find(uuid); it != m_uuidToHandle.end())
		{
			// 返す
			return it->second;
		}

		// 新規のUUIDならパスを取得
		const std::wstring& path = m_dataBase.GetPath(uuid);

		// パスから読み込む
		UnTypeHandle handle = LoadFromPath(path);

		// マップに追加
		m_uuidToHandle[uuid] = handle;

		// 返す
		return handle;
	}

	UnTypeHandle AssetManager::GetHandle(UUID uuid) const
	{
		// マップにあれば
		if (auto it = m_uuidToHandle.find(uuid); it != m_uuidToHandle.end())
		{
			// 返す
			return it->second;
		}

		// なければエラー値(generationが0)を返す
		return UnTypeHandle(0, 0);
	}

	UUID AssetManager::GetUUID(const UnTypeHandle& handle) const
	{
		// Handleに対応するAssetがあればそのUUIDを返す
		if (AssetBase* asset = m_registry.GetFromUnTypeHandle(handle)) return asset->GetUUID();

		// なければエラー値として0を返す
		return 0;
	}

	UnTypeHandle AssetManager::LoadFromPath(const std::wstring& path)
	{
		// ローダー関数を取得
		auto& loader = m_loaders.at(m_typeManager.GetAssetClass(path));

		// asyncで非同期ロード
		auto future = std::async(
			std::launch::async, loader, path
		);

		// Handleを生成
		UnTypeHandle handle = m_registry.Register(m_dataBase.GetUUID(path));

		// futureを配列に追加
		m_asyncJobs.push_back(AsyncJob{ handle.index, std::move(future) });

		// 生成したHandleを返す
		return handle;
	}
}	// namespace REngine
