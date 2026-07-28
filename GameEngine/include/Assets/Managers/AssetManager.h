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
			size_t index;

			// Asyncから受け取るfuture
			std::future<std::unique_ptr<AssetBase>> future;
		};

		// パスを引数にしてAssetを生成するローダー関数
		using Loader = std::function<std::unique_ptr<AssetBase>(const std::wstring&)>;

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

		// 非同期実行中のローダーを管理する配列
		std::vector<AsyncJob> m_asyncJobs;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetManager() = default;
		~AssetManager() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化関数
		void Initialize(const std::wstring& root);

		// 更新関数
		void Update();

		// UUIDから読み込む関数
		UnTypeHandle LoadFromUUID(UUID uuid);

		/// <summary>
		/// Assetの登録を行う関数
		/// </summary>
		/// <typeparam name="T">Assetの型</typeparam>
		/// <param name="loader">読み込み関数</param>
		/// <param name="extentions">対応拡張子の一覧</param>
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<AssetBase, T>>>
		void Registry(const std::string& assetName, Loader loader, const std::vector<std::wstring>& extentions);

		// データベースの取得関数
		const AssetDataBase& GetDataBase() const { return m_dataBase; }

		// タイプ管理者の取得関数
		const AssetTypeManager& GetTypeManager() const { return m_typeManager; }

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// Handle経由で本体の取得をする関数
		template<typename T>
		T* Get(Handle<T> handle)
		{
			// Registryを経由して返す
			return m_registry.Get<T>(handle);
		}

		// Handleが対応するAssetを解放する関数
		template<typename T>
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
		template<typename T>
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

	template<typename T, typename>
	inline void AssetManager::Registry(const std::string& assetName, Loader loader, const std::vector<std::wstring>& extentions)
	{
		// 読み込み関数の登録
		m_loaders[std::type_index(typeid(T))] = loader;

		// 拡張子の登録
		for (auto& ext : extentions)
		{
			m_typeManager.RegisterAssetClass<T>(ext);

			m_typeManager.RegisterAssetType(ext, assetName);
		}
	}
}	// namespace REngine
