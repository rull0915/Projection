/**
 * @file    RendererType.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/26
 *
 * @brief   レンダラーのタイプの列挙型を宣言したヘッダ
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief 描画タイプ
	 */
	enum class RendererType
	{
		None,
		Primitive,
		Mesh,
		Sprite,
		Text,
		UI,
	};
}	// namespace RHEngine
