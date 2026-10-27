/**
 * @file    AssetAsyncLoader.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/12
 *
 * @brief   アセットの非同期読み込みを行うクラス
 */

#include "pch.h"
#include "Assets/Managers/AssetAsyncLoader.h"

#include <future>

namespace RHEngine
{
	AssetAsyncLoader::AssetAsyncLoader(AssetContext& assetCtx)
		: m_assetContext{ assetCtx }
	{}

	void AssetAsyncLoader::RequestLoad(const std::filesystem::path& path, UnTypeHandle targetHandle, Loader loader)
	{
		CleanUpFutures();

		auto future = std::async(
			std::launch::async,
			[this, path, targetHandle, loader]()
			{
				LoadResult result = loader(path, m_assetContext);

				if (result.asset)
				{
					result.asset->SetName(path.stem().string());
				}

				result.handle = targetHandle;

				{
					std::lock_guard<std::mutex> lock(m_mutex);

					m_completedList.push_back(std::move(result));
				}
			}
		);

		// std::async の future は破棄時に完了を待つ。ここで捨てると同期処理になってしまうため、リストに保持して非同期のまま走らせる		
		{
			std::lock_guard<std::mutex> lock(m_futureMutex);
			m_futures.push_back(std::move(future));
		}
	}

	std::vector<LoadResult> AssetAsyncLoader::PopCompletedJobs()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		std::vector<LoadResult> completedJobs = std::move(m_completedList);

		// ムーブ後の状態は未規定のため、明示的に空にしておく
		m_completedList.clear();

		return completedJobs;
	}

	void AssetAsyncLoader::CleanUpFutures()
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
}
