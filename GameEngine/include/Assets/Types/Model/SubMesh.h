/**
 * @file    SubMesh.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/07
 *
 * @brief   サブメッシュ
 */

#pragma once

#include <string>
#include <Windows.h>

#include "Math/Bounding.h"

namespace RHEngine
{
	/**
	 * @brief サブメッシュ
	 */
	struct SubMesh
	{
		/** @brief 描画するインデックス数 */
		UINT indexCount;

		/** @brief 何番目のインデックスから描画を始めるか */
		UINT indexOffset;

		/** @brief 頂点バッファの頂点を読み取る際に全てのインデックスに対して加算される値 */
		INT vertexOffset;

		/** @brief SubMeshを覆う境界ボックス */
		/** @TODO ロード時にBoudingBoxを生成して渡すように */
		BoundingBox boudingBox;

		/** @brief 対応するマテリアル名 */
		std::string materialName;
	};
}	// namespace RHEngine
