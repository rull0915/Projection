//====================================================//
// ファイル名   : UUID.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : UUIDの型を宣言したヘッダ
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

// インクルードの追加
#include <cstdint>

namespace REngine
{
	// UUIDの型を宣言
	using UUID = std::uint64_t;
}	// namespace REngine
