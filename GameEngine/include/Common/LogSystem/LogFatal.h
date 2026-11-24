/**
 * @file    LogFatal.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/18
 *
 * @brief   致命的なログが発生した時の処理
 */

#pragma once

namespace RHEngine
{
	/**
	 * @brief 致命的なエラーでプロジェクトを異常終了させる
	 * @param msg 終了メッセージ
	 */
	[[noreturn]] void FatalExit(const char* msg);
}
