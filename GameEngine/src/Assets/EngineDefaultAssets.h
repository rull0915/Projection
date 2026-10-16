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

namespace REngine
{
	namespace DefaultAsset
	{
		//-------- Shaders --------//
		const UUID LIT_VS = 0x01;
		const UUID LIT_PS = 0x02;

		const UUID SHADOW_PASS_VS = 0x03;

		const UUID PRIMITIVE_VS = 0x05;

		//-------- Materials -------- //
		const UUID BASIC_MATERIAL = 0x11;
		const UUID PRIMITIVE_MATERIAL = 0x12;

		//-------- Sprite --------//
		const UUID BASIC_SPRITE = 0x21;
	}
}
