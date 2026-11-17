/**
 * @file    Logger.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/16
 *
 * @brief   ログの管理をするクラス
 */

#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <string>
#include <atomic>
#include <cstdint>
#include <format>
#include <utility>
#include <source_location>

#include "Log.h"

namespace RHEngine
{
	/**
	 * @brief ログの生成/管理
	 * @details スレッドセーフにするためにmutexを使用する
	 */
	class Logger
	{
	private:

		/** @brief 管理中のSink一覧 */
		std::vector<std::unique_ptr<ILogSink>> m_sinks;

		/** @brief Sinkを保護するmutex */
		std::mutex m_mutex;

		/**
		 * @brief フレーム数
		 * @details 複数スレッドから加算などを行っても大丈夫なようにatomicを使用する
		 */
		std::atomic<uint64_t> m_frameCount;

		/**
		 * @brief 出力するログの最低レベル
		 * @details min未満のログについては取り扱わないようになる
		 */
		std::atomic<LogLevel> m_minLevel;

	public:

		/// @name ライフサイクル
		/// @{

		Logger() = default;

		/// @}

		/**
		 * @brief インスタンス取得
		 * @return 静的インスタンス
		 */
		static Logger& Instance()
		{
			static Logger instance;
			return instance;
		}

		/**
		 * @brief 管理対象のSinkを追加する
		 * @tparam T Sinl型
		 * @param ...args 引数リスト
		 * @return 追加したSinkの生ポインタ
		 */
		template<class T, class... Args>
		T* AddSink(Args&&... args);

		/**
		 * @brief Sinkをリセットする
		 * @details 終了時に明示的に呼び出すこと
		 */
		void ClearSinks();

		/**
		 * @brief フレーム数のカウントを進める
		 * @details フレームの最初で必ず1回呼ぶこと
		 */
		void NextFrame() { ++m_frameCount; }

		/**
		 * @brief 明示的にFlushを呼び出す
		 */
		void Flush();

		/** @brief 処理する最低レベルをセットする */
		void SetMinLevel(LogLevel level) { m_minLevel = level; }

		/** @brief 処理する最低レベルを取得する */
		LogLevel GetMinLevel() const { return m_minLevel; }

		/**
		 * @brief ログを1つ生成する
		 * @tparam ...Args 
		 * @param lv ログレベル
		 * @param category カテゴリ名
		 * @param loc 呼び出し場所の情報
		 * @param fmt 
		 * @param ...args 
		 */
		template<class... Args>
		static void Log(LogLevel lv, const char* category, std::source_location loc,
			std::format_string<Args...> fmt, Args&&... args)
		{
			if (lv < m_minLevel) return;

			Dispatch(lv, category, loc, std::format(fmt, std::forward<Args>(args)...));
		}

	private:

		/**
		 * @brief ログを発行する
		 * @param lv ログレベル
		 * @param category カテゴリ名
		 * @param loc ログを発行した場所の情報
		 * @param msg ログメッセージ本文
		 */
		void Dispatch(LogLevel lv, const char* category, std::source_location loc, std::string msg);
	};

	template<class T, class ...Args>
	inline T* Logger::AddSink(Args && ...args)
	{
		// 生成
		auto sink = std::make_unique<T>(std::forward<Args>(args)...);
		T* raw = sink.get();

		std::lock_guard lk(m_mutex);
		m_sinks.push_back(std::move(sink));
		return raw;
	}
}
