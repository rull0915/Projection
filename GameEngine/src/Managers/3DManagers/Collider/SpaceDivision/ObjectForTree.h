/**
 * @file    ObjectForTree.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/08
 *
 * @brief   分木へ登録するオブジェクトです
 */

#pragma once

#include "Components/World/Collider/3D/ColliderBase.h"

namespace RHEngine
{
	class Cell;

	/**
	 * @brief 分木へ登録するコライダーを持つオブジェクト
	 */
	class ObjectForTree
	{
	public:

		/** @brief 所属しているセル */
		Cell* m_pCell;

		/** @brief 対応するコライダー */
		ColliderBase* m_pObject;

		/** @brief 前のオブジェクト */
		ObjectForTree* m_pPre;

		/** @brief 次のオブジェクト */
		ObjectForTree* m_pNext;

	public:

		/// @name ライフサイクル
		/// @{

		ObjectForTree()
			: m_pCell{ nullptr }
			, m_pObject{ nullptr }
			, m_pPre{ nullptr }
			, m_pNext{ nullptr }
		{}

		virtual ~ObjectForTree() = default;

		/// @}

	public:
		
		/**
		 * @brief 自身を削除する
		 * @return 成功したかどうか
		 */
		bool Remove();

		/**
		 * @brief 所属空間を変更する
		 * @param pCell 
		 */
		void RegistCell(Cell* pCell)
		{
			m_pCell = pCell;
		}

		/**
		 * @brief 次のオブジェクトへのポインタを取得する
		 */
		ObjectForTree* GetNextObj() const {
			return m_pNext;
		}
	};
}	// namespace RHEngine
