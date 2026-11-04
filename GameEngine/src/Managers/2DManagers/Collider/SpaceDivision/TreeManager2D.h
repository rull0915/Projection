/**
 * @file    TreeManager2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/08
 *
 * @brief   4分木管理クラス
 */

#pragma once

#include <cstdint>
#include <vector>
#include <SimpleMath.h>

#include "Cell2D.h"
#include "ObjectForTree2D.h"
#include "Components/World/Collider/2D/ColliderBase2D.h"
#include "Math/Bounding.h"

namespace RHEngine
{
	/**
	 * @brief 4分木を管理する
	 */
	class TreeManager2D
	{
	private:

		/** @brief 作成時に指定できる最大の分割レベル数 */
		static constexpr unsigned int MANAGER_MAX_LEVEL = 8;

		/** @brief 管理しているセルのリスト */
		std::vector<Cell2D*> m_cells;

		/** @brief ルート空間のサイズ */
		DirectX::SimpleMath::Vector2 m_rootSize;

		/** @brief ルート空間の中心座標 */
		DirectX::SimpleMath::Vector2 m_rootPosition;

		/** @brief ルート空間の最小座標 */
		DirectX::SimpleMath::Vector2 m_rootMin;

		/** @brief ルート空間の最大座標 */
		DirectX::SimpleMath::Vector2 m_rootMax;

		/** @brief 最小領域のサイズ */
		DirectX::SimpleMath::Vector2 m_minimumSize;

		/** @brief 最大分割数 */
		unsigned int m_maxLevel;

		/** @brief セルの合計数 */
		unsigned int m_cellCount;

		/** @brief 2の計算済みべき乗のキャッシュ */
		int m_pow[MANAGER_MAX_LEVEL];

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param size ルート空間のサイズ
		 * @param maxLevel 最大分割数
		 * @param center ルート空間の中心
		 */
		TreeManager2D(DirectX::SimpleMath::Vector2 size, unsigned int maxLevel, DirectX::SimpleMath::Vector2 center = { 0, 0 });

		~TreeManager2D();

		/// @}
		
		/**
		 * @brief オブジェクトを登録する
		 * @param obj 対象のオブジェクト
		 */
		void AddObject(ObjectForTree2D* obj);

		/**
		 * @brief 全ての衝突判定リストを調べる 
		 * @param condition 衝突するかどうかを返す関数
		 * @param action 衝突可能性があった時に呼ばれる関数
		 */
		template <typename TCondition, typename TAction>
		void CheckAllCollisionList(TCondition condition, TAction action);

		/**
		 * @brief 衝突判定リストを再帰的に調べる
		 * @param Elem 調べるセルインデックス
		 * @param ColStac そのセルと衝突する可能性があるオブジェクトリスト
		 * @param condition 衝突するかどうかを返す関数
		 * @param action 衝突可能性があった時に呼ばれる関数
		 * @return 成功したかどうか
		 */
		template <typename TCondition, typename TAction>
		bool CheckCollisionList(unsigned int Elem, std::vector<ColliderBase2D*>& ColStac, TCondition condition, TAction action);

		/**
		 * @brief 新しいセルを生成する
		 * @param morton 生成先のモートン番号
		 */
		void CreateCell(unsigned int morton);

		/**
		 * @brief BoundingBoxが収まるセルのモートン番号を取得する
		 * @param box 対象のボックス
		 * @return モートン番号
		 */
		unsigned int GetMortonNumber(const BoundingBox2D& box);

		/**
		 * @brief 最小空間の座標からモートン番号を取得する
		 * @return モートン番号
		 */
		unsigned int GetMortonNumber(uint8_t x, uint8_t y);

		/**
		 * @brief bitを1つ2bit感覚にずつ分割する
		 * @details 1111 -> 01010101 | 1011 -> 01000101
		 * @param bit 対象
		 * @return 分割後のbit
		 */
		unsigned int BitDivision(uint8_t bit);

		/**
		 * @brief 座標からモートン番号を求める
		 * @param point 対象の座標
		 * @return モートン番号
		 */
		unsigned int GetMinimumIndex(const DirectX::SimpleMath::Vector2& point);
	};

	template<typename TCondition, typename TAction>
	inline void TreeManager2D::CheckAllCollisionList(TCondition condition, TAction action)
	{
		// ルート空間の存在をチェック
		if (!m_cells[0])
			return;	// 空間が存在していない

		// ルート空間を処理
		std::vector<ColliderBase2D*> ColStac;
		CheckCollisionList(0, ColStac, condition, action);
	}

	template<typename TCondition, typename TAction>
	inline bool TreeManager2D::CheckCollisionList(unsigned int Elem, std::vector<ColliderBase2D*>& ColStac, TCondition condition, TAction action)
	{
		// 空間内のオブジェクト同士の衝突リスト作成
		ObjectForTree2D* pObj1 = m_cells[Elem]->GetFirstObject();

		// 1つ目がなくなるまでループ
		while (pObj1 != nullptr)
		{
			ObjectForTree2D* pObj2 = pObj1->m_pPre;
			while (pObj2 != nullptr)
			{
				ColliderBase2D* a = pObj1->m_pObject;
				ColliderBase2D* b = pObj2->m_pObject;

				pObj2 = pObj2->m_pPre;

				if (condition(a, b))
				{
					// 実行
					action(a, b);
				}
			}
			// 衝突スタックとの衝突リスト作成
			for (auto& col : ColStac)
			{
				ColliderBase2D* a = pObj1->m_pObject;
				ColliderBase2D* b = col;

				if (condition(a, b))
				{
					// 実行
					action(a, b);
				}
			}
			pObj1 = pObj1->m_pPre;
		}

		bool ChildFlag = false;
		// 子空間に移動
		unsigned int ObjNum = 0;
		unsigned int i, NextElem;
		for (i = 0; i < 4; i++)
		{
			NextElem = Elem * 4 + 1 + i;
			if (NextElem < m_cellCount && m_cells[Elem * 4 + 1 + i])
			{
				if (!ChildFlag)
				{
					// 登録オブジェクトをスタックに追加
					pObj1 = m_cells[Elem]->GetFirstObject();
					while (pObj1)
					{
						ColStac.push_back(pObj1->m_pObject);
						ObjNum++;
						pObj1 = pObj1->m_pPre;
					}
				}
				ChildFlag = true;
				CheckCollisionList(Elem * 4 + 1 + i, ColStac, condition, action);	// 子空間へ
			}
		}

		// スタックからオブジェクトを外す
		if (ChildFlag)
		{
			for (i = 0; i < ObjNum; i++) ColStac.pop_back();
		}

		return true;
	}
}	// namespace RHEngine
