/**
 * @file    CollideEventSystem.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/26
 *
 * @brief   衝突イベントの関数呼び出しを担当するクラス
 */

#pragma once

#include <unordered_map>

#include "Physics/HitContact.h"

namespace RHEngine
{
	class ColliderBase;
	class ColliderBase2D;

	/**
	 * @brief 衝突イベントの関数呼び出しを担当する
	 */
	class CollideEventSystem
	{
	private:

		/** @brief 前フレームの衝突状況 */
		std::unordered_map<ObjectPair, HitContact, ObjectPairHash> m_prevMap;

		/** @brief 前フレームの衝突状況 (2D世界) */
		std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D> m_prevMap2D;

	public:

		/// @name ライフサイクル
		/// @{

		CollideEventSystem()
			: m_prevMap{}
			, m_prevMap2D{}
		{};

		~CollideEventSystem() {};

		/// @}

	public:
		
		/** @brief 前フレームのイベントをクリアする */
		void ResetEvent()
		{
			m_prevMap.clear();
		}

		/**
		 * @brief コライダーを削除する
		 * @details 削除したコライダーに対して衝突イベントの呼び出しが起きることを防ぐ
		 */
		void RemoveCollider(ColliderBase* pCollider);

		/**
		 * @brief 2Dコライダーを削除する
		 */
		void RemoveCollider2D(ColliderBase2D* pCollider);

		/**
		 * @brief 衝突イベントを呼び出す
		 * @details 前フレームの衝突状況と比較して、Enter, Stay, Exit関数を適切に呼び分ける
		 * @param nowMap 今のフレームの衝突状況
		 */
		void CallCollideFunctions(std::unordered_map<ObjectPair, HitContact, ObjectPairHash>& nowMap);

		/**
		 * @brief 2Dの衝突イベントを呼び出す
		 * @param nowMap 今のフレームの衝突状況
		 */
		void CallCollideFunctions2D(std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D>& nowMap);

	private:

		void OnEnter(HitContact& contact);
		void OnStay(HitContact& contact);
		void OnExit(HitContact& contact);

		void OnEnter2D(HitContact2D& contact);
		void OnStay2D(HitContact2D& contact);
		void OnExit2D(HitContact2D& contact);
	};
}	// namespace RHEngine
