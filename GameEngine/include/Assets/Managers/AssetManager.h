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
#include <any>
#include <future>
#include <typeindex>

#include "Assets/Objects/AssetBase.h"
#include "Assets/Objects/Handle.h"
#include "AssetDataBase.h"
#include "AssetRegistry.h"
#include "AssetTypeManager.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetManager
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
		std::unordered_map<UUID, std::any> m_uuidToHandle;

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
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<AssetBase, T>>>
		Handle<T> LoadFromUUID(UUID uuid);

		/// <summary>
		/// Assetの登録を行う関数
		/// </summary>
		/// <typeparam name="T">Assetの型</typeparam>
		/// <param name="loader">読み込み関数</param>
		/// <param name="extentions">対応拡張子の一覧</param>
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<AssetBase, T>>>
		void Registry(const std::string& assetName, Loader loader, const std::vector<std::wstring>& extentions);

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
			if (T* asset = m_registry.Get(h)) return asset->uuid;

			// なければエラー値として0を返す
			return 0;
		}

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// パスから読み込む関数
		template<typename T>
		Handle<T> LoadFromPath(const std::wstring& path);
	};

	template<typename T, typename>
	inline Handle<T> AssetManager::LoadFromUUID(UUID uuid)
	{
		// 既に読み込まれているUUIDなら
		if (auto it = m_uuidToHandle.find(uuid); it != m_uuidToHandle.end())
		{
			// 変換し返す
			return std::any_cast<Handle<T>>(it->second);
		}

		// 新規のUUIDならパスを取得
		const std::wstring& path = m_dataBase.GetPath(uuid);

		// パスから読み込む
		Handle<T> handle = LoadFromPath(path);

		// ハンドルに対応したAssetのUUIDを設定する
		if (AssetBase* asset = m_registry.Get<T>(handle))
		{
			asset->SetUUID(uuid);
		}

		// マップに追加
		m_uuidToHandle[uuid] = handle;

		// 返す
		return handle;
	}

	template<typename T, typename>
	inline void AssetManager::Registry(const std::string& assetName, Loader loader, const std::vector<std::wstring>& extentions)
	{
		// 読み込み関数の登録
		m_loaders[std::type_index(typeid(T))] = loader;

		// 拡張子の登録
		for (auto& ext : extentions)
		{
			m_typeManager.RegisterAssetType(ext, assetName);
		}
	}

	template<typename T>
	inline Handle<T> AssetManager::LoadFromPath(const std::wstring& path)
	{
		// ローダー関数を取得
		auto& loader = m_loaders.at(std::type_index(typeid(T)));

		// asyncで非同期ロード
		auto future = std::async(
			std::launch::async, &loader, path
		);

		// Handleを生成
		Handle<T> handle = m_registry.Register<T>();

		// futureを配列に追加
		m_asyncJobs.push_back(AsyncJob{ handle.index, future });

		// 生成したHandleを返す
		return handle;
	}
}	// namespace REngine
