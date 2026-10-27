/**
 * @file    CBufferSlot.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/06
 *
 * @brief   定数バッファの規約をまとめたヘッダ
 */

#pragma once
#include <cstdint>

namespace RHEngine
{
	/**
	 * @brief 定数バッファのスロット規約
	 */
	namespace CBufferSlot
	{
		constexpr uint32_t PerFrame = 0;	// b0はフレーム毎に更新されるバッファ
		constexpr uint32_t PerCamera = 1;	// b1はカメラごとに更新されるバッファ
		constexpr uint32_t PerObject = 2;	// b2はオブジェクト毎に更新されるバッファ
		constexpr uint32_t MaterialRangeStart = 3; // 3番以降は全てMaterialの管轄
	}

	/**
	 * @brief テクスチャバッファのスロット規約
	 */
	namespace TBufferSlot
	{
		constexpr uint32_t MaterialRangeStart = 10; // 10番未満は全てMaterialの管轄
	}

	/**
	 * @brief サンプラーバッファのスロット規約
	 */
	namespace SBufferSlot
	{
		constexpr uint32_t MaterialRangeStart = 10; // 10番未満は全てMaterialの管轄
	}

	/**
	 * @brief マテリアル管轄の定数バッファスロットか調べる
	 * @param slot 対象のスロット番号
	 */
	static bool IsMaterialManagedCSlot(uint32_t slot) { return slot >= CBufferSlot::MaterialRangeStart; }

	/**
	 * @brief マテリアル管轄のテクスチャバッファスロットか調べる
	 * @param slot 対象のスロット番号
	 */
	static bool IsMaterialManagedTSlot(uint32_t slot) { return slot < TBufferSlot::MaterialRangeStart; }

	/**
	 * @brief マテリアル管轄のサンプラーバッファスロットか調べる
	 * @param slot 対象のスロット番号
	 */
	static bool IsMaterialManagedSSlot(uint32_t slot) { return slot < SBufferSlot::MaterialRangeStart; }
}	// namespace RHEngine
