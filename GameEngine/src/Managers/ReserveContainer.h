/**
 * @file    ReserveContainer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/02
 *
 * @brief   オブジェクトの追加/削除を遅延させるコンテナクラス
 */

#pragma once

#include <vector>
#include <unordered_set>

namespace RHEngine
{
	class Renderer;
	class Scene;

	/**
	 * @brief オブジェクトの追加削除を遅延させるコンテナ
	 * @details 任意のタイミングで追加・削除予約を反映させることで、forループが壊れてしまうことやダングリング参照を防ぐことが出来る
	 * @tparam T 管理する型
	 */
	template<typename T>
	class ReserveContainer
	{
	private:

		/** @brief リスト本体 */
		std::vector<T*> m_objectList;

		/** @brief 追加予約の一覧 */
		std::vector<T*> m_addReserves;

		/** @brief 削除予約の一覧 */
		std::unordered_set<T*> m_removeReserves;

	public:

		// @name ライフサイクル
		/// @{

		ReserveContainer()
			: m_objectList{}
			, m_addReserves{}
			, m_removeReserves{}
		{
		}

		virtual ~ReserveContainer() = default;

		/// @}

		/**
		 * @brief オブジェクトを追加する
		 */
		void AddObject(T* c) { m_addReserves.push_back(c); }

		/**
		 * @brief オブジェクトを削除する
		 */
		void RemoveObject(T* c) { m_removeReserves.insert(c); }

		/**
		 * @brief 管理中の全てのオブジェクトを取得する
		 */
		const std::vector<T*>& GetObjects() const { return m_objectList; }

		/**
		 * @brief 管理中の全てのオブジェクトを取得する
		 */
		std::vector<T*>& GetObjects() { return m_objectList; }

		/**
		 * @brief 追加予約を反映する
		 */
		virtual void AddReserved()
		{
			for (auto p : m_addReserves)
			{
				m_objectList.push_back(p);
			}

			m_addReserves.clear();
		}

		/**
		 * @brief 削除予約を反映する
		 */
		virtual void RemoveReserved()
		{
			// 削除リストが空なら何もしない
			if (m_removeReserves.empty()) return;

			// 削除リストに含まれているかを調べるラムダ式
			auto shouldRemove = [this](T* base)
				{
					return m_removeReserves.contains(base);
				};

			// 削除
			std::erase_if(m_objectList, shouldRemove);		
			std::erase_if(m_addReserves, shouldRemove);

			// 削除リストをクリア
			m_removeReserves.clear();
		}

		/**
		 * @brief 追加・削除予約を反映する
		 */
		void ReflectReserves()
		{
			AddReserved();
			RemoveReserved();
		}

		/**
		 * @brief オブジェクトをリセットする
		 */
		void Reset()
		{
			m_addReserves.clear();
			m_objectList.clear();
			m_removeReserves.clear();
		}

	protected:

		/**
		 * @brief 追加予約リストを取得する
		 */
		std::vector<T*>& GetAddReserves()
		{
			return m_addReserves;
		}
		/**
		 * @brief 削除予約リストを取得する
		 */
		std::unordered_set<T*>& GetRemoveReserves()
		{
			return m_removeReserves;
		}
	};
}	// namespace RHEngine
