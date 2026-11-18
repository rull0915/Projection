/**
 * @file    Log.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/11
 *
 * @brief   ロギングシステムに使用するヘッダ
 */

#pragma once

#include <cstdint>
#include <string>

namespace RHEngine
{
	/**
	 * @brief ログのレベル
	 * @details 下に行けば行くほど重要度が高い
	 */
	enum class LogLevel : uint8_t
	{
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Fatal,

		Count
	};

	/**
	 * @brief ログレベルから文字列を取得する関数
	 * @param lv 取得対象のレベル
	 */
	inline const char* ToString(LogLevel lv)
	{
		static constexpr const char* names[] = { "TRACE","DEBUG","INFO","WARNING","ERROR","FATAL" };
		return names[static_cast<size_t>(lv)];
	}

	/**
	 * @brief 1ログの情報
	 */
	struct LogEntry
	{
		LogLevel    level;		///< ログレベル
		std::string message;	///< 本文
		std::string file;       ///< ファイル名のみ
		uint32_t    line;		///< 行数
		std::string time;       ///< "12:34:56.789"
		uint64_t    frame;		///< フレーム
		uint32_t    threadId;	///< スレッド
	};

	/**
	 * @brief ログをため込むインターフェース
	 */
	class ILogSink
	{
	public:

		/// @name ライフサイクル
		/// @{

		virtual ~ILogSink() = default;

		/// @}

		/**
		 * @brief 受け取ったログを出力先に書き込む
		 * @param entry 対象のログ情報
		 */
		virtual void Write(const LogEntry& entry) = 0;

		/**
		 * @brief ため込んだログを出力先に反映させる
		 */
		virtual void Flush() {};
	};
}	// namespace RHEngine
