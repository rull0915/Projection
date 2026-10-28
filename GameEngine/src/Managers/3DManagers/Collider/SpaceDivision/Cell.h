/**
 * @file    Cell.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/08
 *
 * @brief   分木空間クラス
 */

#pragma once

#include "ObjectForTree.h"

namespace RHEngine
{
	/**
	 * @brief 分木空間のセル
	 */
	class Cell
	{
	private:

		/** @brief 最新オブジェクトへのポインタ */
		ObjectForTree* m_pLatest;

	public:

		/// @name ライフサイクル
		/// @{

		Cell()
			: m_pLatest{ nullptr }
		{};

		~Cell() = default;

		/// @}

		/**
		 * @brief オブジェクトをセルへ追加する
		 * @param pObject 対象のオブジェクト
		 * @return 成功したかどうか
		 */
		bool AddObject(ObjectForTree* pObject)
		{
			// nullをスキップ
			if (!pObject) return false;

			// 登録済みをスキップ
			if (pObject->m_pCell == this) return false;

			// オブジェクトが未登録なら
			if (!m_pLatest)
			{
				m_pLatest = pObject;
			}
			// オブジェクトが登録済みなら
			else
			{
				// リストへ追加
				m_pLatest->m_pNext = pObject;   // 最新の次を追加
				pObject->m_pPre = m_pLatest;    // 追加されるポインタの前を登録
				m_pLatest = pObject;            // 最新を更新
			}

			// オブジェクトを自身に所属させる
			m_pLatest->m_pCell = this;
			return true;
		}

		/**
		 * @brief オブジェクト削除時の処理
		 * @param pObject 削除されるオブジェクト
		 * @return 成功したかどうか
		 */
		bool OnRemove(ObjectForTree* pObject)
		{
			if (m_pLatest == pObject) {
				// 前のオブジェクトに挿げ替え
				m_pLatest = m_pLatest->m_pPre;
			}
			return true;
		}

		/**
		 * @brief オブジェクトのつながりを削除する
		 * @param pObject 対象のオブジェクト
		 */
		void ResetLink(ObjectForTree*& pObject)
		{
			// 次が存在するとき
			if (pObject->m_pNext)
			{
				ResetLink(pObject->m_pNext);
			}
			pObject = nullptr;
		}

		/**
		 * @brief 最新オブジェクトのポインタを取得する
		 */
		ObjectForTree* GetFirstObject() { return m_pLatest; }
	};
}	// namespace RHEngine
