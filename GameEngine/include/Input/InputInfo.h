//====================================================//
// ファイル名   : InputInfo.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要 : 入力関連の情報をまとめたヘッダ
//
// 更新履歴 :
// 2026/06/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <variant>

#include "Input/KeyInput.h"
#include "Input/MouseInput.h"
#include "Input/PadInput.h"

namespace REngine
{
	namespace Input
	{
		// キーIDの型指定
		using ID = unsigned char;

		// 押下状況
		enum class State
		{
			Down,
			Press,
			Up,
		};

		// ボタン対応variant
		using InputButton = std::variant<
			Key::Code,
			Mouse::Button,
			Pad::Button>;

		// Axis対応
		using InputAxis = std::variant<
			Mouse::Axis,
			Pad::Axis
		>;
	}
}	// namespace REngine
