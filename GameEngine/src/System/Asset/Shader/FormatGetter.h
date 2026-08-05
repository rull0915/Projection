//====================================================//
// ファイル名   : FormatGetter.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/04
//
// 概要 : フォーマットを取得する関数
//
// 更新履歴 :
// 2026/08/04 新規作成
//====================================================//

#pragma once
#include "dxgiformat.h"
#include "d3dcommon.h"
#include "Windows.h"

//====================================================//
// インクルードファイル
//====================================================//

namespace REngine
{
	namespace Format
	{
		// タイプとマスクからフォーマットを取得する関数
		DXGI_FORMAT GetFormat(
			D3D_REGISTER_COMPONENT_TYPE type,
			BYTE mask);
	}
}
