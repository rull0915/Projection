/**
 * @file    EngineDefaultAssets.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/13
 *
 * @brief   ゲームエンジン側のデフォルトで用意されているアセットのIDをまとめたヘッダ
 * @details 10000未満のインデックスはゲームエンジンデフォルトアセットの領域とする
 */

#pragma once

#include "Common/UUID.h"

namespace RHEngine
{
	namespace DefaultAsset
	{
		//-------- Shaders --------//
		const UUID LIT_VS = 1;
		const UUID LIT_PS = 2;

		const UUID SHADOW_PASS_VS = 3;

		const UUID PRIMITIVE_VS = 5;
		const UUID PRIMITIVE_PS = 6;

		const UUID SPRITE_PS = 7;
		const UUID SPRITE_VS = 8;

		//-------- Materials -------- //
		const UUID BASIC_MATERIAL = 11;
		const UUID PRIMITIVE_MATERIAL = 12;
		const UUID SPRITE_MATERIAL = 13;

		//-------- Sprite --------//
		const UUID DEFAULT_SPRITE = 21;
		const UUID ENGINE_LOGO_SPRITE = 22;
	}
}	// namespace RHEngine
