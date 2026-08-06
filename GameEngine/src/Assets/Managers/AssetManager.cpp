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

#include <algorithm>

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	AssetManager::AssetManager()
		: m_dataBase{ m_typeManager }
		, m_registry{}
		, m_typeManager{}
		, m_uuidToHandle{}
		, m_loaders{}
		, m_savers{}
		, m_creators{}
		, m_creatableAssets{}
		, m_asyncJobs{}
	{}

	void AssetManager::Initialize(const std::wstring& root)
	{
		// スキャン
		m_dataBase.ScanFile(root);
	}

	void AssetManager::ScanOnceFile(const std::wstring& path)
	{
		// スキャン
		m_dataBase.ScanOnceFile(path);
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

		// 無効値ならエラーハンドルを返す
		if (uuid == UUID_NONE) return ERROR_UNTYPE_HANDLE;

		// 新規のUUIDならパスを取得
		const std::wstring& path = m_dataBase.GetPath(uuid);

		// パスから読み込む
		UnTypeHandle handle = LoadFromPath(path);

		// マップに追加
		m_uuidToHandle[uuid] = handle;

		// 返す
		return handle;
	}

	void AssetManager::Create(const std::filesystem::path& directory, const std::string& fileName, const std::string& assetType)
	{
		// 対応していないタイプなら何もしない
		if (std::find(m_creatableAssets.begin(), m_creatableAssets.end(), assetType) == m_creatableAssets.end()) return;

		// 拡張子を取得
		std::wstring ext = m_typeManager.GetExtention(assetType);

		// パスを作成
		std::filesystem::path path = directory / (fileName);
		path += ext;

		// タイプインデックスを取得
		std::type_index idx = m_typeManager.GetAssetClass(path);

		// 生成関数を取得
		const auto& creator = m_creators.find(idx);

		// あれば
		if (creator->second)
		{
			// 生成
			std::unique_ptr<AssetBase> asset = std::move(creator->second(path));

			// 保存関数を取得
			const auto& saver = m_savers.find(idx);

			// あれば
			if (saver->second)
			{
				// 保存
				saver->second(asset.get(), path);
			}

			// スキャンさせる
			m_dataBase.ScanOnceFile(path);
		}
	}

	bool AssetManager::CanSave(const std::filesystem::path& path)
	{
		auto it = m_savers.find(m_typeManager.GetAssetClass(path));

		return it != m_savers.end();
	}

	void AssetManager::SaveAsset(const std::filesystem::path& path)
	{
		// 保存関数を取得
		auto it = m_savers.find(m_typeManager.GetAssetClass(path));

		if (it == m_savers.end()) return;

		// UUIDを取得
		UUID uuid = m_dataBase.GetUUID(path);

		// ハンドルを取得
		auto handle = m_uuidToHandle.find(uuid);

		if (handle == m_uuidToHandle.end()) return;

		// アセット本体を取得
		AssetBase* asset = m_registry.GetFromUnTypeHandle(handle->second);

		if (!asset) return;

		// 保存
		it->second(asset, path);
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
