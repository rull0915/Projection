/**
 * @file    VertexElementInfo.h
 * @author  Hoshino Ryunosuke
 * @date    2026/09/24
 *
 * @brief   InputLayout作成時に使用する情報をまとめたヘッダ
 */

#pragma once

#include <string>
#include <dxgiformat.h>

namespace RHEngine
{
	/**
	 * @brief InputLayout作成時に使用する構造体
	 */
	struct VertexElementInfo
	{
		/** @brief セマンティック名 */
		std::string semanticName;

		/** @brief セマンティックインデックス (同名のセマンティックが複数ある場合の識別用番号) */
		unsigned int semanticIndex;

		/** @brief CPU側のメモリフォーマット */
		DXGI_FORMAT format;

		/** @brief バイトオフセット (基本的にoffsetofマクロを使って計算) */
		unsigned int byteOffset;

		/** @brief 頂点バッファのスロット番号 */
		unsigned int inputSlot = 0;
	};
}	// namespace RHEngine
