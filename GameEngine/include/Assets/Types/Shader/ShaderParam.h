/**
 * @file    ShaderParam.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/03
 *
 * @brief   シェーダーのパラメータ
 */

#pragma once

#include <cstdint>
#include <string>

namespace RHEngine
{
	/**
	 * @brief シェーダーのパラメータタイプ
	 */
	enum class ShaderParamType
	{
		None,
		Float,
		Float2,
		Float3,
		Float4,
		Color,
		Float4x4,
		Texture2D,
		Sampler,
	};

	/**
	 * @brief 1つのパラメータ
	 */
	struct ShaderParam
	{
		std::string name;		///< 名前
		ShaderParamType type;	///< タイプ

		uint32_t size;		///< サイズ
		uint32_t offset;	///< オフセット

		uint32_t slot;	///< スロット番号
	};

	/**
	 * @brief 1スロットの定数バッファの情報
	 */
	struct ConstantBufferInfo
	{
		uint32_t slot;	///< 定数バッファのスロット番号 register(bN)
		uint32_t size;	///< 定数バッファ全体の合計サイズ
	};
}	// namespace RHEngine
