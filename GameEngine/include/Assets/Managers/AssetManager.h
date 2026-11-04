/**
 * @file    AssetManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   Assetの管理をするクラス
 */

#pragma once

#include <string>
#include <functional>
#include <fstream>
#include <memory>
#include <typeindex>

#include "Assets/Objects/AssetBase.h"
#include "Assets/Objects/Handle.h"
#include "IAssetResolver.h"
#include "AssetDataBase.h"
#include "AssetRegistry.h"
#include "AssetTypeManager.h"
#include "AssetContext.h"
#include "AssetAsyncLoader.h"

namespace RHEngine
{
	/** @brief アセットであることを制限するconcept */
	template<typename T>
	concept AssetType = std::derived_from<T, AssetBase>;

	/**
	 * @brief アセットの管理を行う
	 */
	class AssetManager : public IAssetResolver
	{
	private:

		/** @brief パスを引数にしてAssetを生成するローダー関数名 */
		using Loader = AssetAsyncLoader::Loader;

		/** @brief Assetからパスに保存するセーバー関数 */
		using Saver = std::function<void(AssetBase*, const std::filesystem::path&, AssetContext& ctx)>;

		/** @brief パスから新規ファイルを作成してデフォルトのアセットを生成する関数 */
		using Creator = std::function<std::unique_ptr<AssetBase>(std::filesystem::path)>;

	private:

		/** @brief アセットとUUIDのデータベース */
		AssetDataBase m_dataBase;

		/** @brief Asset本体のレジストリ */
		AssetRegistry m_registry;

		/** @brief Assetタイプの管理 */
		AssetTypeManager m_typeManager;

		/** @brief Asset読み書き時に渡すコンテキスト */
		AssetContext m_assetContext;

		/** @brief Asset非同期読み込み */
		AssetAsyncLoader m_asyncLoader;

		/** @brief タイプからロード関数への変換マップ */
		std::unordered_map<std::type_index, Loader> m_loaders;

		/** @brief タイプから保存関数への変換マップ */
		std::unordered_map<std::type_index, Saver> m_savers;

		/** @brief タイプから生成関数への変換マップ */
		std::unordered_map<std::type_index, Creator> m_creators;

		/** @brief 新規作成が可能なアセットタイプのリスト */
		std::vector<std::string> m_creatableAssets;

		/** @brief サブアセットを持つアセットタイプのリスト */
		std::vector<std::string> m_haveSubAssets;

	public:
		/// @name ライフサイクル
		/// @{
		 
		AssetManager();

		~AssetManager();

		/// @}

		/**
		 * @brief 初期化関数
		 * @details フォルダのスキャン・デフォルトアセットの登録を行う
		 * @param root スキャンしてUUIDの登録を行うルートフォルダ
		 */
		void Initialize(const std::filesystem::path& root);

		/**
		 * @brief 更新関数
		 * @details 非同期ロードの完了チェックを行う
		 */
		void Update();

		/**
		 * @brief UUIDからアセット本体をロードする
		 * @param uuid ロード対象のUUID
		 * @return ロードしたアセットに対応するハンドル
		 */
		UnTypeHandle LoadFromUUID(UUID uuid);

		/**
		 * @brief アセットの登録をする関数
		 * @tparam T 登録対象のアセット型
		 * @param assetName アセット名
		 * @param loader ロード関数
		 * @param saver 保存関数
		 * @param canCreate 新規作成可能かどうか
		 * @param haveSubAsset サブアセットを持つかどうか
		 * @param extentions 対応する拡張子のリスト
		 */
		template<AssetType T>
		void Registry(
			const std::string& assetName,
			Loader loader,
			Saver saver,
			bool canCreate,
			bool haveSubAsset,
			const std::vector<std::wstring>& extentions);

		/**
		 * @brief アセットを新規作成する
		 * @param directory 作成するフォルダ
		 * @param fileName 作成するファイル名
		 * @param assetType 作成するアセット名
		 */
		void Create(const std::filesystem::path& directory, const std::string& fileName, const std::string& assetType);

		/**
		 * @brief アセットが保存可能か調べる
		 * @param path 調査対象のアセットファイルパス
		 * @return 保存可能かどうか
		 */
		bool CanSave(const std::filesystem::path& path);

		/**
		 * @brief アセットがサブアセットを持つかどうか調べる
		 * @param path 調査対象のアセットファイルパス
		 * @return サブアセットを持つかどうか
		 */
		bool HaveSubAsset(const std::filesystem::path& path);

		/**
		 * @brief アセットを保存する
		 * @param path 保存先のパス
		 */
		void SaveAsset(const std::filesystem::path& path);

		/**
		 * @brief ハンドルから本体を取得する
		 * @tparam T 取得するアセット型
		 * @param handle 取得するハンドル
		 * @return アセット本体
		 */
		template<AssetType T>
		T* Get(Handle<T> handle) const
		{
			// Registryを経由して返す
			return m_registry.Get<T>(handle);
		}

		/**
		 * @brief ハンドルから本体を取得する (タイプ識別なし)
		 * @param handle 取得するハンドル
		 * @return アセット本体
		 */
		AssetBase* GetFromUnTypeHandle(UnTypeHandle handle)
		{
			// Registryを経由して返す
			return m_registry.GetFromUnTypeHandle(handle);
		}

		/**
		 * @brief アセットを解放する
		 * @tparam T アセット型
		 * @param handle 解放するアセットハンドル
		 */
		template<AssetType T>
		void Release(Handle<T> handle)
		{
			// ハンドルに対応するAssetがあれば削除
			if (Get(handle)) m_registry.Release(handle.index);
		}

		/**
		 * @brief アセットハンドルからUUIDを取得する
		 * @tparam T アセット型
		 * @param h アセットハンドル
		 * @return 対応するUUID
		 */
		template<AssetType T>
		UUID GetUuidFromHandle(Handle<T> h)
		{
			// Handleに対応するAssetがあればそのUUIDを返す
			if (T* asset = m_registry.Get(h)) return asset->GetUUID();

			// なければエラー値として0を返す
			return 0;
		}

		/**
		 * @brief UUIDからハンドルを取得する
		 * @param uuid 調べるUUID
		 * @return UUIDに対応するハンドル
		 */
		UnTypeHandle GetHandle(UUID uuid) const override
		{
			return m_registry.GetHandle(uuid);
		}

		/**
		 * @brief ハンドルからUUIDを取得する
		 * @param handle 調べるハンドル
		 * @return 対応するUUID
		 */
		UUID GetUUID(const UnTypeHandle& handle) const override
		{
			return m_registry.GetUUID(handle);
		}

		/**
		 * @brief UUIDから直接アセット本体を取得する
		 * @tparam T 取得するアセット型
		 * @param id 調べるUUID
		 * @return 対応するアセット本体
		 */
		template<typename T>
		T* GetAssetFromUUID(UUID id) const
		{
			return m_registry.Get(m_registry.GetHandle(id).As<T>());
		}

		/// @name ゲッター
		/// @{

		/** @brief データベースの取得 */
		AssetDataBase& GetDataBase() { return m_dataBase; }

		/** @brief アセットタイプ管理者の取得 */
		const AssetTypeManager& GetTypeManager() const { return m_typeManager; }

		/** @brief 生成可能なアセットタイプのリストを取得する */
		const std::vector<std::string>& GetCreatableAssets() const { return m_creatableAssets; }

		/// @}

	private:

		/**
		 * @brief ファイルパスからアセットを読み込む
		 * @param path 読み込むファイルパス
		 * @return 読み込んだアセットに対応するハンドル
		 */
		UnTypeHandle LoadFromPath(const std::filesystem::path& path);
	};

	template<AssetType T>
	inline void AssetManager::Registry(const std::string& assetName, Loader loader, Saver saver, bool canCreate, bool haveSubAsset, const std::vector<std::wstring>& extentions)
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

						// デフォルトコンストラクタで作成
						return std::make_unique<T>();
					};

				// 生成可能に追加
				m_creatableAssets.push_back(assetName);
			}
		}

		if (haveSubAsset)
		{
			m_haveSubAssets.push_back(assetName);
		}

		// 拡張子の登録
		for (auto& ext : extentions)
		{
			m_typeManager.RegisterAssetClass<T>(ext);

			m_typeManager.RegisterAssetType(ext, assetName);
		}
	}
}	// namespace RHEngine
