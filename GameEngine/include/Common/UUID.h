/**
 * @file    UUID.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   UUIDの型を宣言したヘッダ
 */

#pragma once

#include <cstdint>

namespace RHEngine
{
	using UUID = std::uint64_t;

	static constexpr UUID UUID_NONE = 0;
}	// namespace RHEngine
