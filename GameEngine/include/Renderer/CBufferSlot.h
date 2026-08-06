//====================================================//
// ファイル名   : CBufferSlot.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/06
//
// 概要 : 定数バッファの規約をまとめたヘッダ
//		 このプロジェクトで定数バッファを使用する際は以下の規約に則ってください。
//
// 更新履歴 :
// 2026/08/06 新規作成
//====================================================//

#pragma once
#include <cstdint>

namespace REngine
{
	namespace CBufferSlot
	{
		constexpr uint32_t PerFrame = 0; // View/Projection
		constexpr uint32_t PerObject = 1; // World行列
		constexpr uint32_t MaterialRangeStart = 2; // 2番以降は全てMaterialの管轄
	}

	/// <summary>
	/// マテリアルが管轄するスロットかを調べる関数
	/// </summary>
	/// <param name="slot">スロット番号</param>
	/// <returns></returns>
	static bool IsMaterialManagedSlot(uint32_t slot) { return slot >= CBufferSlot::MaterialRangeStart; }
}	// namespace REngine
