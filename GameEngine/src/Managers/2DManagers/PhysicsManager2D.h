/**
 * @file    PhysicsManager2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/16
 *
 * @brief   2D版物理挙動管理クラス
 */

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Components/World/RigidBody/RigidBody2D.h"
#include "Physics/HitContact.h"
#include "../ReserveContainer.h"

namespace RHEngine
{
	class CollideManager2D;

	/**
	 * @brief 2D版物理挙動管理クラス
	 */
	class PhysicsManager2D : public ReserveContainer<RigidBody2D>
	{
	private:

		/** @brief 衝突判定の管理者 */
		std::unique_ptr<CollideManager2D> m_collideManager;

		/** @brief 衝突情報のリスト */
		std::vector<HitContact2D> m_contacts;

		/** @brief 衝突したペアとその情報のマップ */
		std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D> m_contactMap;

	public:

		/// @name ライフサイクル
		/// @{

		PhysicsManager2D();

		~PhysicsManager2D();

		/// @}

		/**
		 * @brief 更新処理 全Rigidbodyの積分を行う
		 * @param elapsedTime 経過時間
		 */
		void Update(float elapsedTime);

		/**
		 * @brief 衝突情報のリストを全て取得する
		 */
		std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D>& GetHitList() { return m_contactMap; }

		/**
		 * @brief 衝突管理クラスを取得する
		 */
		CollideManager2D* GetCollideManager() { return m_collideManager.get(); }

	private:
		/**
		 * @brief 衝突後の補正を行う
		 */
		void HittedCorrection();

		/**
		 * @brief 衝突後の位置補正を行う
		 * @param contact 対象の衝突情報
		 */
		void PositionCorrection(HitContact2D& contact);

		/**
		 * @brief 衝突後の速度補正を行う
		 * @param contact 対象の衝突情報
		 */
		void VelocityCorrection(HitContact2D& contact);
	};
}	// namespace RHEngine
