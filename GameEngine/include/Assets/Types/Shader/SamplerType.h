/**
 * @file    SamplerType.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/08
 *
 * @brief   サンプラーの種類をまとめた列挙型
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief サンプラーの種類を表す列挙型
	 */
	enum class SamplerType
	{
		None,
		LinearWrap,
		PointWrap,
		LinearClamp,
		PointClamp,
		Anisotropic,
	};
}	// namespace RHEngine
