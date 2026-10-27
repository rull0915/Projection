/**
 * @file    EventBus.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/17
 *
 * @brief   イベントバス
 */

#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <type_traits>

namespace RHEngine
{
	/**
	 * @brief イベントバスを管理する
	 * @tparam EventID イベントの識別用列挙型
	 */
	template<typename EventID, typename = std::enable_if_t<std::is_enum_v<EventID>>>
	class EventBus
	{
	public:
		using CallBack = std::function<void()>;

		/**
		 * @brief イベント識別用トークン
		 */
		struct Token
		{
			unsigned int id;	///< ID
			EventID type;		///< イベントのタイプ
		};

	private:
		
		/**
		 * @brief イベントリスナー
		 */
		struct Listener
		{
			unsigned int id;	// ID
			CallBack callback;	// イベント発行時の処理
		};

	private:

		/** @brief 各イベントに登録されているリスナーの一覧 */
		static inline std::unordered_map<EventID, std::vector<Listener>> m_events{};

		/** @brief 登録予約中のリスナー */
		static inline std::unordered_map<EventID, std::vector<Listener>> m_addReserves{};

		/** @brief 現在管理中のID */
		static inline unsigned int m_nowID = 0;

		/** @brief 発行の再帰数 */
		static inline size_t m_publishDepth = 0;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @details 静的クラスのためインスタンス化を禁止
		 */
		EventBus() = delete;

		~EventBus() = delete;

		/// @}

		/**
		 * @brief リスナー登録関数
		 * @param id 対象のイベントID
		 * @param call イベント発行時のコールバック関数
		 * @return イベント識別用トークン (削除時に使用)
		 */
		static Token Register(EventID id, CallBack call)
		{
			// 新規IDを生成
			m_nowID += 1;

			// 実行中でなければ
			if (m_publishDepth == 0)
			{
				// 追加
				m_events[id].push_back({ m_nowID, call });
			}
			// 実行中なら
			else
			{
				// 予約リストに追加
				m_addReserves[id].push_back({ m_nowID, call });
			}

			return { m_nowID, id };
		}

		/**
		 * @brief リスナーを削除する
		 * @param token 登録時に発行された識別用トークン
		 */
		static void Remove(Token token)
		{
			// ID
			unsigned int id = token.id;
			EventID eventId = token.type;

			// 指定されたタイプのイベントリストを取得
			auto it = m_events.find(eventId);

			// あれば
			if (it != m_events.end())
			{
				// 全イベントを調べる
				for (size_t i = 0; i < it->second.size(); ++i)
				{
					// idが一致したら
					if (it->second[i].id == id)
					{
						// 削除予約として設定
						it->second[i].id = 0;
					}
				}
			}

			// 予約リストも調査
			auto addIt = m_addReserves.find(eventId);

			// あれば
			if (addIt != m_addReserves.end())
			{
				// 全イベントを調べる
				for (size_t i = 0; i < addIt->second.size(); ++i)
				{
					// idが一致したら
					if (addIt->second[i].id == id)
					{
						// 削除予約として設定
						addIt->second[i].id = 0;
					}
				}
			}

			// 実行中でなければ
			if (m_publishDepth == 0)
			{
				// 削除予約をリストから削除
				if (it != m_events.end()) std::erase_if(it->second, [](const Listener& func) { return func.id == 0; });

				// 予約リスト
				if (addIt != m_addReserves.end()) std::erase_if(addIt->second, [](const Listener& func) { return func.id == 0; });
			}
		}

		/**
		 * @brief イベントを発行する
		 * @param eventId 発行するイベントのID
		 */
		static void Publish(EventID eventId)
		{
			// 実行がされていなければ
			if (m_publishDepth == 0)
			{
				// 予約されている処理を追加
				RegisterReserved();
			}

			// 指定されたタイプのイベントリストを取得
			auto it = m_events.find(eventId);

			// なければスキップ
			if (it == m_events.end()) return;

			// 保護用変数のインクリメント
			++m_publishDepth;

			// 登録されている処理を実行
			for (auto& func : it->second)
			{
				// 削除予約中なら非実行
				if (func.id == 0) continue;

				// 実行
				func.callback();
			}

			// デクリメント
			--m_publishDepth;

			// 全実行が終わっていれば
			if (m_publishDepth == 0)
			{
				// 削除予約をリストから削除
				CleanUp();
			}
		}

	private:
		/**
		 * @brief 登録予約中のリスナー登録する
		 */
		static void RegisterReserved()
		{
			// 全予約をループ
			for (auto& it : m_addReserves)
			{
				for (auto& func : it.second)
				{
					// 既に削除予約済みなら登録しない
					if (func.id == 0) continue;

					// 追加
					m_events[it.first].push_back(func);
				}
			}

			// クリア
			m_addReserves.clear();
		}

		/**
		 * @brief 削除対象のリスナーをリストから削除する
		 */
		static void CleanUp()
		{
			// 全イベントをループ
			for (auto& [id, vec] : m_events)
			{
				std::erase_if(vec,
					[](auto& x)
					{
						return x.id == 0;
					});
			}

			// 予約リストも調査
			for (auto& [id, vec] : m_addReserves)
			{
				std::erase_if(vec,
					[](auto& x)
					{
						return x.id == 0;
					});
			}
		}
	};
} // namespace RHEngine
