/**
 * @file    AuxFileRepository.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   AssetAuxから.auxファイルの読み書きを行うクラス
 */

#pragma once

#include "Assets/Objects/AssetAux.h"

namespace RHEngine
{
	/**
	 * @brief AssetAuxから.auxファイルへ読み書きを行う
	 */
	class AuxFileRepository
	{
	public:

		/// @name ライフサイクル
		/// @{
		 
		AuxFileRepository() = default;

		~AuxFileRepository() = default;

		/// @}

		/**
		 * @brief .auxファイルへ保存する
		 * @param aux 保存する対象のAssetAux
		 * @param path 保存先のファイルパス
		 */
		void SaveAux(const AssetAux& aux, const std::wstring& path);

		/**
		 * @brief .auxから読み込む
		 * @param aux 読み込み先のAssetAux
		 * @param path 読み込むファイルパス
		 */
		void LoadAux(AssetAux& aux, const std::wstring& path);
	};
}	// namespace RHEngine
