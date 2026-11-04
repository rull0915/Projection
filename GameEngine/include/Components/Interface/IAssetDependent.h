/**
 * @file    IAssetDependent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   Assetに依存することを明示するインターフェース
 */

#pragma once

namespace RHEngine
{
	class AssetManager;

	/**
	 * @brief アセットに依存するコンポーネントのインターフェース
	 */
	class IAssetDependent
	{
	public:
		/// @name ライフサイクル
		/// @{

		IAssetDependent() = default;

		virtual ~IAssetDependent() = default;

		/// @}

		/**
		 * @brief Assetマネージャーを受け取る
		 * @details AddComponentの際にシーン経由で渡される
		 * @param assets アセットマネージャー
		 */
		virtual void ReceiveAssetManager(AssetManager& assets) = 0;
	};
}	// namespace RHEngine
