/**
 * @file    InputInfo.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 *
 * @brief   入力関連の情報をまとめたヘッダ
 */

#pragma once

#include <variant>

#include "Input/KeyInput.h"
#include "Input/MouseInput.h"
#include "Input/PadInput.h"

namespace RHEngine
{
	namespace Input
	{
		/**
		 * @brief IDをucharに指定
		 */
		using ID = unsigned char;

		/**
		 * @brief 押下状況
		 */
		enum class State
		{
			Down,
			Press,
			Up,
		};

		/**
		 * @brief 全てのボタンに対応するvariant
		 */
		using InputButton = std::variant<
			Key::Code,
			Mouse::Button,
			Pad::Button>;

		/**
		 * @brief 全ての軸入力に対応するvariant
		 */
		using InputAxis = std::variant<
			Mouse::Axis,
			Pad::Axis
		>;
	}
}	// namespace RHEngine
