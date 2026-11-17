/**
 * @file    ImGuiLogSink.h
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 *
 * @brief   ImGuiで扱うログシンク
 */

#pragma once

#include "Log.h"

#include <deque>
#include <mutex>

namespace RHEngine
{
	class ImGuiLogSink : public ILogSink
	{
	private:

		/** @brief ログエントリ一覧保護用のmtx */
		std::mutex m_mutex;
		
		/** @brief 発行されたログ情報の一覧 */
		std::deque<LogEntry> m_entries;

		/** @brief 保持しておくログの最大数 */
		size_t m_capacity;

	public:

		/// @name ライフサイクル
		/// @{
		
		explicit ImGuiLogSink(size_t capacity = 5000) 
			: m_mutex{}
			, m_entries{}
			, m_capacity(capacity) 
		{}

		/// @}

		/**
		 * @brief エントリを増やす
		 * @param e 対象のエントリ
		 */
		void Write(const LogEntry& e) override
		{
			std::lock_guard lk(m_mutex);	// 保護
			if (m_entries.size() >= m_capacity) m_entries.pop_front();   // 古いものから捨てる
			m_entries.push_back(e);
		}

		/**
		 * @brief エントリリストをクリアする
		 */
		void Clear() 
		{
			std::lock_guard lk(m_mutex); 
			m_entries.clear(); 
		}

		/**
		 * @brief エントリへのアクセス関数
		 * @tparam F 関数オブジェクト型
		 * @param f 実行される処理
		 * @details m_entriesをロックするために関数オブジェクトを渡してもらう方式にする
		 */
		template<class F> void Access(F&& f)
		{
			std::lock_guard lk(m_mutex);
			f(m_entries);
		}
	};
}
