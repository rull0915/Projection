/**
 * @file    ObjectForTree2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/16
 *
 * @brief   分木が管理するオブジェクト
 */

#pragma once

#include "Components/World/Collider/2D/ColliderBase2D.h"

namespace RHEngine
{
	class Cell2D;

	/**
	 * @brief 分木が管理する2Dコライダーを持つオブジェクト
	 */
	class ObjectForTree2D
	{
	public:

		/** @brief 自身が所属しているセル */
		Cell2D* m_pCell;

		/** @brief 対応するコライダー */
		ColliderBase2D* m_pObject;

		/** @brief 自分の前のオブジェクト */
		ObjectForTree2D* m_pPre;

		/** @brief 自分の次のオブジェクト */
		ObjectForTree2D* m_pNext;

	public:

		/// @name ライフサイクル
		/// @{

		ObjectForTree2D()
			: m_pCell{ nullptr }
			, m_pObject{ nullptr }
			, m_pPre{ nullptr }
			, m_pNext{ nullptr }
		{}

		virtual ~ObjectForTree2D()
		{}

		/// @}

	public:
		/**
		 * @brief 自身を削除する
		 * @return 成功したかどうか
		 */
		bool Remove();

		/**
		 * @brief 所属空間を登録する
		 * @param pCell 
		 */
		void RegistCell(Cell2D* pCell)
		{
			m_pCell = pCell;
		}

		/**
		 * @brief 次のオブジェクトへのポインタを取得する
		 */
		ObjectForTree2D* GetNextObj() const {
			return m_pNext;
		}
	};
}	// namespace RHEngine
