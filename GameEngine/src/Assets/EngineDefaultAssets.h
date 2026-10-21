//====================================================//
// ファイル名   : EngineDefaultAssets.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/13
//
// 概要 : ゲームエンジン側のデフォルトで用意されているアセットのIDをまとめたヘッダ
//			1万未満のIDはデフォルトアセットが使用します
//
// 更新履歴 :
// 2026/10/13 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "Common/UUID.h"

namespace RHEngine
{
	namespace DefaultAsset
	{
		//-------- Shaders --------//
		const UUID LIT_VS = 01;
		const UUID LIT_PS = 02;

		const UUID SHADOW_PASS_VS = 03;

		const UUID PRIMITIVE_VS = 05;
		const UUID PRIMITIVE_PS = 06;

		//-------- Materials -------- //
		const UUID BASIC_MATERIAL = 11;
		const UUID PRIMITIVE_MATERIAL = 12;

		//-------- Sprite --------//
		const UUID DEFAULT_SPRITE = 21;
		const UUID ENGINE_LOGO_SPRITE = 22;
	}
}
