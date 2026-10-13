//====================================================//
// ファイル名  : AssetAsyncLoader.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/12
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"

#include "Assets/Managers/AssetAsyncLoader.h"
#include <future>

//====================================================//
// 関数の実体宣言
//====================================================//

void REngine::AssetAsyncLoader::RequestLoad(const std::filesystem::path& path, UnTypeHandle targetHandle, Loader& loader)
{
	// 完了したfutureを削除
	CleanUpFutures();

	// asyncで非同期ロード
	auto future = std::async(
		std::launch::async,
		[this, path, targetHandle, &loader]()
		{
			LoadResult result = loader(path, m_assetContext);

			if (result.asset)
			{
				result.asset->SetName(path.stem().string());
			}

			result.handle = targetHandle;

			{
				// ロック
				std::lock_guard<std::mutex> lock(m_mutex);

				// ロード完了ジョブに追加
				m_completedList.push_back(std::move(result));
			}
		}
	);

	// リストに保存してデストラクタでの即時ブロックを回避
	{
		std::lock_guard<std::mutex> lock(m_futureMutex);
		m_futures.push_back(std::move(future));
	}
}
