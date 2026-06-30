#include <functional>
#include <unordered_map>
#include <vector>
#include <type_traits>

template<typename EventID, typename = std::enable_if_t<std::is_enum_v<EventID>>>
class EventBus
{
public:
	using CallBack = std::function<void()>;

	//----------------------------------------------
	// メンバ変数
	//----------------------------------------------
private:

	// 呼び出し関係マップ
	static inline std::unordered_map<EventID, std::vector<std::pair<unsigned int, CallBack>>> m_events{};

	// 登録待ち
	static inline std::unordered_map<EventID, std::vector<std::pair<unsigned int, CallBack>>> m_addReserves{};

	// 現在のID
	static inline unsigned int m_nowID = 0;

	// 実行中フラグ
	static inline size_t m_publishDepth = 0;

	//----------------------------------------------
	// 関数宣言
	//----------------------------------------------
public:

	EventBus() = delete;	// 静的クラスのためインスタンス化禁止

	// 登録関数
	static unsigned int Register(EventID id, CallBack call)
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

		return m_nowID;
	}

	// 削除関数
	static void Remove(EventID eventId, unsigned int id)
	{
		// 指定されたタイプのイベントリストを取得
		auto it = m_events.find(eventId);

		// あれば
		if (it != m_events.end())
		{
			// 全イベントを調べる
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				// idが一致したら
				if (it->second[i].first == id)
				{
					// 削除予約として設定
					it->second[i].first = 0;
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
				if (addIt->second[i].first == id)
				{
					// 削除予約として設定
					addIt->second[i].first = 0;
				}
			}
		}

		// 実行中でなければ
		if (m_publishDepth == 0)
		{
			// 削除予約をリストから削除
			if (it != m_events.end()) std::erase_if(it->second, [](const std::pair<unsigned int, CallBack>& func) { return func.first == 0; });

			// 予約リスト
			if (addIt != m_addReserves.end()) std::erase_if(addIt->second, [](const std::pair<unsigned int, CallBack>& func) { return func.first == 0; });
		}
	}

	// 実行関数
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
			if (func.first == 0) continue;

			// 実行
			func.second();
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
	// 登録待ちを追加する関数
	static void RegisterReserved()
	{
		// 全予約をループ
		for (auto& it : m_addReserves)
		{
			for (auto& func : it.second)
			{
				// 既に削除予約済みなら登録しない
				if (func.first == 0) continue;

				// 追加
				m_events[it.first].push_back(func);
			}
		}

		// クリア
		m_addReserves.clear();
	}

	// 全削除予約を配列から削除する関数
	static void CleanUp()
	{
		// 全イベントをループ
		for (auto& [id, vec] : m_events)
		{
			std::erase_if(vec,
				[](auto& x)
				{
					return x.first == 0;
				});
		}
		
		// 予約リストも調査
		for (auto& [id, vec] : m_addReserves)
		{
			std::erase_if(vec,
				[](auto& x)
				{
					return x.first == 0;
				});
		}
	}
};
