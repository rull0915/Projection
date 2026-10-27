/**
 * @file    Prefab.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/27
 *
 * @brief   プレハブアセット
 */

#pragma once

#include <memory>
#include <filesystem>

#include "Assets/Objects/AssetBase.h"

namespace RHEngine
{
	class PrefabImpl;

	/**
	 * @brief プレハブアセット
	 */
	class Prefab : public AssetBase
	{
	private:

		/**
		 * @brief 実装本体
		 * @details オブジェクトを表すjson構造体を保持する
		 */
		std::unique_ptr<PrefabImpl> m_impl;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param path 読み込み対象のファイルパス
		 */
		Prefab(std::filesystem::path path);

		/**
		 * @brief implを直接渡す方式のコンストラクタ
		 * @param impl 実装本体
		 */
		Prefab(std::unique_ptr<PrefabImpl> impl);

		~Prefab() = default;

		/// @}

		/** @brief 実装本体を取得する */
		PrefabImpl* GetImpl() const { return m_impl.get(); }
	};
}	// namespace RHEngine
