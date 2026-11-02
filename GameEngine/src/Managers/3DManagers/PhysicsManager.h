/**
 * @file    PhysicsManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/08
 *
 * @brief   物理挙動管理クラスです
 */

#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "Components/World/RigidBody/RigidBody.h"
#include "Physics/HitContact.h"
#include "../ReserveContainer.h"

namespace RHEngine
{
	class CollideManager;

	/**
	 * @brief 3D物理挙動管理クラス
	 */
	class PhysicsManager : public ReserveContainer<RigidBody>
	{
	private:

		/** @brief 衝突管理クラス */
		std::unique_ptr<CollideManager> m_collideManager;

		/** @brief 衝突情報のリスト */
		std::vector<HitContact> m_contacts;

		/** @brief 衝突ペアと衝突情報の対応マップ */
		std::unordered_map<ObjectPair, HitContact, ObjectPairHash> m_contactMap;

	public:

		/// @name ライフサイクル
		/// @{

		PhysicsManager();

		~PhysicsManager();

		/// @}

		/**
		 * @brief 全Rigidbodyの積分を行う
		 * @param elapsedTime 経過時間
		 */
		void Update(float elapsedTime);

		/**
		 * @brief 衝突情報のリストを全て取得する
		 */
		std::unordered_map<ObjectPair, HitContact, ObjectPairHash>& GetHitList() { return m_contactMap; }

		/**
		 * @brief 衝突管理クラスを取得する
		 */
		CollideManager* GetCollideManager() { return m_collideManager.get(); }

	private:
		/**
		 * @brief 衝突後の補正を行う
		 */
		void HittedCorrection();

		/**
		 * @brief 衝突後の位置補正を行う
		 * @param contact 対象の衝突情報
		 */
		void PositionCorrection(HitContact& contact);

		/**
		 * @brief 衝突後の速度補正を行う
		 * @param contact 対象の衝突情報
		 */
		void VelocityCorrection(HitContact& contact);
	};
}	// namespace RHEngine
