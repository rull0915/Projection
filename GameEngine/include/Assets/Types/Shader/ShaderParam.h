//====================================================//
// ファイル名   : ShaderParam.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/03
//
// 概要 : シェーダーのパラメータ
//
// 更新履歴 :
// 2026/08/03 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <cstdint>

namespace REngine
{
	enum class ShaderParamType
	{
		None,
		Float,
		Float2,
		Float3,
		Float4,
		Float4x4,
		Texture2D,
	};

	// 1つのパラメータ
	struct ShaderParam
	{
		std::string name;		// 名前
		ShaderParamType type;	// タイプ

		uint32_t size;		// サイズ
		uint32_t offset;	// オフセット

		uint32_t cbSlot;	// 定数バッファのスロット番号	register(bN)
		uint32_t tSlot;		// テクスチャのスロット番号		register(tN)
		uint32_t uSlot;		// register(uN) 現在は未使用
	};

	// 定数バッファ全体の情報
	struct ConstantBufferInfo
	{
		uint32_t slot;	// 定数バッファのスロット番号 register(bN)
		uint32_t size;	// 定数バッファ全体の合計サイズ
	};
}	// namespace REngine
