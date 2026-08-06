//====================================================//
// ファイル名   : AssetManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : 
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <string>
#include <functional>
#include <fstream>
#include <memory>
#include <future>
#include <typeindex>

#include "Assets/Objects/AssetBase.h"
#include "Assets/Objects/Handle.h"
#include "IAssetResolver.h"
#include "AssetDataBase.h"
#include "AssetRegistry.h"
#include "AssetTypeManager.h"

namespace REngine
{
	// アセットであることを制限するconcept
	template<typename T>
	concept AssetType = std::derived_from<T, AssetBase>;

	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetManager : public IAssetResolver
	{
	private:

		// 非同期読み込みを識別するための構造体
		struct AsyncJob
		{
			// Assetが存在しているindex
			uint32_t index;

			// Asyncから受け取るfuture
			std::future<std::unique_ptr<AssetBase>> future;
		};

		// パスを引数にしてAssetを生成するローダー関数
		using Loader = std::function<std::unique_ptr<AssetBase>(const std::filesystem::path&)>;

		// Assetからパスに保存するセーバー関数
		using Saver = std::function<void(AssetBase*, const std::filesystem::path&)>;

		// パスから新規ファイルを作成してデフォルトのアセットを生成する関数
		using Creator = std::function<std::unique_ptr<AssetBase>(std::filesystem::path)>;

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// AssetDataBase
		AssetDataBase m_dataBase;

		// AssetRegistry
		AssetRegistry m_registry;

		// AssetTypeManager
		AssetTypeManager m_typeManager;

		// UUIDとHandleの対応マップ
		std::unordered_map<UUID, UnTypeHandle> m_uuidToHandle;

		// 読み込み関数マップ
		std::unordered_map<std::type_index, Loader> m_loaders;

		// 保存関数マップ
		std::unordered_map<std::type_index, Saver> m_savers;

		// 生成関数マップ
		std::unordered_map<std::type_index, Creator> m_creators;

		// 生成可能なAssetTypeをまとめた配列
		std::vector<std::string> m_creatableAssets;

		// 非同期実行中のローダーを管理する配列
		std::vector<AsyncJob> m_asyncJobs;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetManager();
		~AssetManager() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize(const std::wstring& root);

		// 特定のファイルのスキャンをする関数
		void ScanOnceFile(const std::wstring& path);

		// 更新関数
		void Update();

		// UUIDから読み込む関数
		UnTypeHandle LoadFromUUID(UUID uuid);

		/// <summary>
		/// Assetの登録をする関数
		/// </summary>
		/// <typeparam name="T">アセットの型</typeparam>
		/// <param name="assetName">アセット名</param>
		/// <param name="loader">ロード関数</param>
		/// <param name="saver">保存関数</param>
		/// <param name="canCreate">実行中に新規作成可能にするかどうか</param>
		/// <param name="extentions">対応する拡張子の一覧</param>
		template<AssetType T>
		void Registry(
			const std::string& assetName, 
			Loader loader, 
			Saver saver,
			bool canCreate,
			const std::vector<std::wstring>& extentions);

		// データベースの取得関数
		AssetDataBase& GetDataBase() { return m_dataBase; }

		// タイプ管理者の取得関数
		const AssetTypeManager& GetTypeManager() const { return m_typeManager; }

		// 生成可能なタイプを全て取得する関数
		const std::vector<std::string>& GetCreatableAssets() const { return m_creatableAssets; }

		// アセットを新規作成する関数
		void Create(const std::filesystem::path& directory, const std::string& fileName, const std::string& assetType);

		// アセットをセーブ可能か調べる関数
		bool CanSave(const std::filesystem::path& path);

		// 保存する関数
		void SaveAsset(const std::filesystem::path& path);

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// Handle経由で本体の取得をする関数
		template<AssetType T>
		T* Get(Handle<T> handle)
		{
			// Registryを経由して返す
			return m_registry.Get<T>(handle);
		}

		// タイプ識別なしで本体を取得する関数
		AssetBase* GetFromUnTypeHandle(UnTypeHandle handle)
		{
			// Registryを経由して返す
			return m_registry.GetFromUnTypeHandle(handle);
		}

		// Handleが対応するAssetを解放する関数
		template<AssetType T>
		void Release(Handle<T> handle)
		{
			// ハンドルに対応するAssetがあれば
			if (Get(handle))
			{
				// Registryを経由して削除
				m_registry.Release(handle.index);

				// UUIDを取得
				UUID uuid = GetUuidFromHandle(handle);

				// マップから削除
				m_uuidToHandle.erase(uuid);
			}
		}

		// HandleからUUIDを取得する関数
		template<AssetType T>
		UUID GetUuidFromHandle(Handle<T> h)
		{
			// Handleに対応するAssetがあればそのUUIDを返す
			if (T* asset = m_registry.Get(h)) return asset->GetUUID();

			// なければエラー値として0を返す
			return 0;
		}

		// UUIDからUnTypeHandleを取得する関数
		UnTypeHandle GetHandle(UUID uuid) const override;

		// HandelからUUIDを取得する関数
		UUID GetUUID(const UnTypeHandle& handle) const override;

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// パスから読み込む関数
		UnTypeHandle LoadFromPath(const std::wstring& path);
	};

	template<AssetType T>
	inline void AssetManager::Registry(const std::string& assetName, Loader loader, Saver saver, bool canCreate, const std::vector<std::wstring>& extentions)
	{
		std::type_index idx = std::type_index(typeid(T));

		// 読み込み関数の登録
		if (loader) m_loaders[idx] = loader;

		// 保存関数の登録
		if (saver) m_savers[idx] = saver;
		
		// デフォルトコンストラクタがあれば
		if constexpr (std::is_default_constructible_v<T>)
		{
			// 生成関数の登録
			if (canCreate)
			{
				m_creators[idx] =
					[](const std::filesystem::path& path)
					{
						// 新規作成
						std::ofstream(path).close();

						// デフォルトコンストラクタで作成（なければエラー）
						return std::make_unique<T>();
					};

				// 生成可能に追加
				m_creatableAssets.push_back(assetName);
			}
		}

		// 拡張子の登録
		for (auto& ext : extentions)
		{
			m_typeManager.RegisterAssetClass<T>(ext);

			m_typeManager.RegisterAssetType(ext, assetName);
		}
	}
}	// namespace REngine
