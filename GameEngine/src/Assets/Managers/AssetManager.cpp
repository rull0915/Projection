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
		, m_assetContext{ m_dataBase, m_registry, [this](UUID uuid) { return LoadFromUUID(uuid); } }
		, m_asyncLoader{ m_assetContext }
		, m_loaders{}
		, m_savers{}
		, m_creators{}
		, m_creatableAssets{}
	{}

	void AssetManager::Initialize(const std::filesystem::path& root)
	{
		// スキャン
		m_dataBase.ScanFile(root);
	}

	void AssetManager::ScanOnceFile(const std::filesystem::path& path)
	{
		// スキャン
		m_dataBase.ScanOnceFile(path);
	}

	void AssetManager::Update()
	{
		//　ロードが完了したJobを取得する
		auto completedJobs = m_asyncLoader.PopCompletedJobs();

		for (auto& job : completedJobs)
		{
			// 失敗していれば追加しない
			if (!job.success || !job.asset) continue;

			// 生成したAssetを取得
			auto asset = job.asset.get();
				
			// Assetのステータスを読み込み済みに変更
			asset->SetStatus(LoadStatus::Loaded);

			// 自身を渡す
			asset->ReceiveAssetManager(this);

			// 読み込み
			asset->OnValidate();

			// 置き換える
			m_registry.Replace(job.handle.index, std::move(job.asset));

			// サブアセットを走査
			for (auto& subAsset : job.subAssets)
			{
				// Assetのステータスを読み込み済みに変更
				subAsset.asset->SetStatus(LoadStatus::Loaded);

				// 読み込み
				subAsset.asset->OnValidate();

				// 自身を渡す
				subAsset.asset->ReceiveAssetManager(this);

				// 置き換える
				m_registry.Replace(subAsset.handle.index, std::move(subAsset.asset));
			}
		}
	}

	UnTypeHandle AssetManager::LoadFromUUID(UUID uuid)
	{
		// エラーUUIDならエラー値を返す
		if (uuid == UUID_NONE) return ERROR_UNTYPE_HANDLE;

		// 既に読み込まれているUUIDなら
		auto handle = m_registry.GetHandle(uuid);
		if (handle != ERROR_UNTYPE_HANDLE) return handle;

		// MainAssetのIDを取得
		UUID mainID = m_dataBase.GetMainUUID(uuid);

		// 既に読み込まれているMainIDかどうかを調べる
		auto mainHandle = m_registry.GetHandle(mainID);
		bool loaded = (mainHandle != ERROR_UNTYPE_HANDLE);

		// まだ読み込んでいなければ
		if (!loaded)
		{
			// MainIDからパスを取得
			const std::filesystem::path& path = m_dataBase.GetPath(mainID);

			// 未登録のUUIDならエラーハンドルを返す
			if (path == L"") return ERROR_UNTYPE_HANDLE;

			// パスから読み込む
			mainHandle = LoadFromPath(path);
		}

		// サブアセットのIDなら
		if (mainID != uuid)
		{
			// サブアセットのハンドルを返す
			return m_registry.Register(uuid);
		}

		// メインアセットのハンドルを返す
		return mainHandle;
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
				saver->second(asset.get(), path, m_assetContext);
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

	bool AssetManager::HaveSubAsset(const std::filesystem::path& path)
	{
		auto it = std::find(m_haveSubAssets.begin(), m_haveSubAssets.end(), m_typeManager.GetAssetType(path));

		return it != m_haveSubAssets.end();
	}

	void AssetManager::SaveAsset(const std::filesystem::path& path)
	{
		// 保存関数を取得
		auto it = m_savers.find(m_typeManager.GetAssetClass(path));

		if (it == m_savers.end()) return;

		// UUIDを取得
		UUID uuid = m_dataBase.GetUUID(path);

		// ハンドルを取得
		auto handle = m_registry.GetHandle(uuid);

		if (handle == ERROR_UNTYPE_HANDLE) return;

		// アセット本体を取得
		AssetBase* asset = m_registry.GetFromUnTypeHandle(handle);

		if (!asset) return;

		// 保存
		it->second(asset, path, m_assetContext);
	}

	UnTypeHandle AssetManager::LoadFromPath(const std::filesystem::path& path)
	{
		if (m_loaders.find(m_typeManager.GetAssetClass(path)) != m_loaders.end())
		{
			// ローダー関数を取得
			auto& loader = m_loaders.at(m_typeManager.GetAssetClass(path));

			// Handleを生成
			UnTypeHandle handle = m_registry.Register(m_dataBase.GetUUID(path));

			// 非同期ロードリクエスト
			m_asyncLoader.RequestLoad(path, handle, loader);

			// 生成したHandleを返す
			return handle;
		}

		return ERROR_UNTYPE_HANDLE;
	}
}	// namespace REngine
