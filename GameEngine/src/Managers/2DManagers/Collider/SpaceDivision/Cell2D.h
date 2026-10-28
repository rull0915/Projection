/**
 * @file    Cell2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/08
 *
 * @brief   2D用分木空間クラス
 */

#include "ObjectForTree2D.h"

namespace RHEngine
{
	class Cell2D
	{
	private:

		/** @brief 最新のオブジェクトへのポインタ */
		ObjectForTree2D* m_pLatest; 

	public:

		/// @name ライフサイクル
		/// @{

		Cell2D()
			: m_pLatest{ nullptr }
		{};

		~Cell2D() = default;

		/// @}

		/**
		 * @brief オブジェクトを追加する
		 * @param pObject 追加対象のオブジェクトを
		 * @return 成功したかどうか
		 */
		bool AddObject(ObjectForTree2D* pObject)
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
		 * @return　成功したかどうか
		 */
		bool OnRemove(ObjectForTree2D* pObject)
		{
			if (m_pLatest == pObject) {
				// 前のオブジェクトに挿げ替え
				m_pLatest = m_pLatest->m_pPre;
			}
			return true;
		}

		/**
		 * @brief オブジェクトの繋がりを解除する
		 * @param pObject 対象のオブジェクト
		 */
		void ResetLink(ObjectForTree2D*& pObject)
		{
			// 次が存在するとき
			if (pObject->m_pNext)
			{
				ResetLink(pObject->m_pNext);
			}
			pObject = nullptr;
		}

		/**
		 * @brief 最新のオブジェクトを取得する
		 */
		ObjectForTree2D* GetFirstObject() { return m_pLatest; }
	};
}	// namespace RHEngine
