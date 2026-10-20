//====================================================//
// ファイル名   : AssetAsyncLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/12
//
// 概要 : アセットの非同期読み込みを行うクラス
//
// 更新履歴 :
// 2026/10/12 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <functional>
#include <filesystem>
#include <mutex>
#include <future>

#include "Assets/Objects/Handle.h"

#include "Assets/Managers/AssetContext.h"
#include "AssetLoadResult.h"

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetAsyncLoader
	{
	public:
		// ローダーの定義
		using Loader = std::function<LoadResult(const std::filesystem::path&, AssetContext& ctx)>;

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 非同期読み込みが完了したJobのリスト
		std::vector<LoadResult> m_completedList;

		// ロック用mutex
		std::mutex m_mutex;

		// アセットコンテキスト
		AssetContext& m_assetContext;

		// 非同期実行中のjobのfutureリスト
		std::mutex m_futureMutex;

		std::vector<std::future<void>> m_futures;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetAsyncLoader(AssetContext& assetCtx)
			: m_assetContext{ assetCtx }
		{}
		~AssetAsyncLoader() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 読み込み関数
		void RequestLoad(const std::filesystem::path& path, UnTypeHandle targetHandle, Loader& loader);

		// ロード完了ジョブを解決する関数
		std::vector<LoadResult> PopCompletedJobs()
		{
			// スレッドセーフに
			std::lock_guard<std::mutex> lock(m_mutex);

			// 完了したジョブを取得
			std::vector<LoadResult> completedJobs = std::move(m_completedList);

			// 完了ジョブリストをクリア
			m_completedList.clear();

			return completedJobs;
		}

	private:
		void CleanUpFutures()
		{
			std::lock_guard<std::mutex> lock(m_futureMutex);

			// 完了したfutureを削除
			std::erase_if(m_futures,
				[](std::future<void>& f)
				{
					return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
				}
			);
		}
	};
}
