//====================================================//
// ファイル名   : ComponentTypeId.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/26
//
// 概要 : コンポーネントのTypeIdを宣言したヘッダ
//
// 更新履歴 :
// 2026/08/26 新規作成
//====================================================//

#pragma once

namespace REngine
{
	namespace Component
	{
		// アドレスをIdとして扱う
		using TypeId = const void*;
	}
}
