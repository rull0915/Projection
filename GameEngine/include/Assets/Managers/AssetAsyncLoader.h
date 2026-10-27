/**
 * @file    AssetAsyncLoader.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/12
 *
 * @brief   アセットの非同期読み込みを行うクラス
 */

#pragma once

#include <functional>
#include <filesystem>
#include <vector>
#include <future>
#include <mutex>

#include "Assets/Objects/Handle.h"
#include "Assets/Managers/AssetContext.h"
#include "AssetLoadResult.h"

namespace RHEngine
{
	/**
	 * @brief アセットの非同期読み込みを行う
	 */
	class AssetAsyncLoader
	{
	public:
		/** @brief ローダー型の定義 */
		using Loader = std::function<LoadResult(const std::filesystem::path&, AssetContext&)>;

	private:

		/** @brief 非同期読み込みが完了したジョブのリスト */
		std::vector<LoadResult> m_completedList;

		/** @brief m_completedListを保護するためのmutex */
		std::mutex m_mutex;

		/** @brief 読み込み時に使用するコンテキスト */
		AssetContext& m_assetContext;

		/** @brief 非同期実行中のジョブのfutureリスト */
		std::vector<std::future<void>> m_futures;

		/** @brief m_futuresを保護するmutex */
		std::mutex m_futureMutex;

	public:

		/// @name ライフサイクル
		/// @{
		
		/**
		 * @param assetCtx 読み込みに使うコンテキスト
		 */
		explicit AssetAsyncLoader(AssetContext& assetCtx);

		AssetAsyncLoader(const AssetAsyncLoader&) = delete;	
		AssetAsyncLoader& operator=(const AssetAsyncLoader&) = delete;
		AssetAsyncLoader(AssetAsyncLoader&&) = delete;
		AssetAsyncLoader& operator=(AssetAsyncLoader&&) = delete;

		/**
		 * @details 実行中の読み込みが全て終わるまでブロックする
		 *			ワーカースレッドがm_mutex / m_completedList / m_assetContextを使うためfutureはこれより後に宣言すること
		 */
		~AssetAsyncLoader() = default;

		/// @}

		/**
		 * @brief アセット読み込みをリクエストする
		 * @details loaderはワーカースレッドで実行される
		 *			ローダー内部で共有変数にアクセスする場合は適切に同期すること
		 * @param path 読み込み対象のファイルパス
		 * @param targetHandle 書き込み対象のハンドル
		 * @param loader ロード関数
		 */
		void RequestLoad(const std::filesystem::path& path, UnTypeHandle targetHandle, Loader loader);

		/**
		 * @brief ロードが完了したジョブを返す
		 * @return ロード完了ジョブリスト
		 */
		std::vector<LoadResult> PopCompletedJobs();

	private:

		/**
		 * @brief 完了したfutureをリストから削除する
		 */
		void CleanUpFutures();
	};
}	// namespace RHEngine
