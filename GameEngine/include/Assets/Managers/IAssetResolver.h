/**
 * @file    IAssetResolver.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/27
 *
 * @brief   アセットの変換を行うインターフェース
 */

#pragma once

#include "Common/UUID.h"
#include "Assets/Objects/Handle.h"

namespace RHEngine
{
	/**
	 * @brief アセットハンドルとUUIDの変換を行うインターフェース
	 */
	class IAssetResolver
	{
	public:
		/// @name ライフサイクル
		/// @{

		IAssetResolver() = default;

		virtual ~IAssetResolver() = default; 

		/// @}

		/**
		 * @brief ハンドルからUUIDを取得する
		 * @param  調べるハンドル
		 * @return 対応するUUID
		 */
		virtual UUID GetUUID(const UnTypeHandle&) const = 0;

		/**
		 * @brief UUIDからハンドルを取得する
		 * @param  調べるUUID
		 * @return 対応するハンドル
		 */
		virtual UnTypeHandle GetHandle(UUID) const = 0;
	};
}	// namespace RHEngine
