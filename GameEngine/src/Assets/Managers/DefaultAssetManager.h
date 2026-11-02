/**
 * @file    DefaultAssetManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/17
 *
 * @brief   デフォルトアセットの管理をするクラス
 */

#pragma once

#include <memory>

#include "Assets/Managers/AssetContext.h"
#include "Assets/Types/Texture.h"

namespace RHEngine
{
	/**
	 * @brief デフォルトアセットを管理する
	 * @details ゲームエンジンに.hファイルとして埋め込まれたデフォルトのアセットを読み込み、アセットレジストリに登録する
	 */
	class DefaultAssetManager
	{
	private:

		/** @brief アセットコンテキスト */
		AssetContext& m_assetContext;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param assetContext アセットコンテキスト (レジスタに登録する際に使用)
		 */
		DefaultAssetManager(AssetContext& assetContext)
			: m_assetContext{ assetContext }
		{
		}

		~DefaultAssetManager() = default;

		/// @}
	
		/**
		 * @brief デフォルトアセットを作成し登録する
		 */
		void CreateAndRegister();

	private:

		/**
		 * @brief テクスチャアセットを作成する
		 * @param data テクスチャデータの先頭アドレス
		 * @param size テクスチャデータのサイズ
		 * @return 作成したテクスチャアセット
		 */
		std::unique_ptr<Texture> CreateTextureAsset(const unsigned char* data, const unsigned int size);
	};
}	// namespace RHEngine
