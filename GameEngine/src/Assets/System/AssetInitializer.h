/**
 * @file    AssetInitializer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   エンジンにデフォルトで存在するアセットの初期化を行うクラス
 */

#pragma once

#include "Assets/Managers/AssetManager.h"

namespace RHEngine
{
	/**
	 * @brief アセットの初期化
	 */
	class AssetInitializer
	{
	public:

		/**
		 * @brief アセットを初期化する
		 * @param assetManager アセットマネージャー
		 */
		static void AssetInitialize(AssetManager& assetManager);
	};
}	// namespace RHEngine
