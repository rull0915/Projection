/**
 * @file    RaySystem.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/26
 *
 * @brief   Rayのシステムを管理するクラス
 */

#pragma once

#include <unordered_map>
#include <cstdint>

#include "Physics/RaycastHit.h"
#include "Physics/Ray.h"
#include "Components/ComponentTypeId.h"

namespace RHEngine
{
	class ColliderBase;

	/**
	 * @brief Rayと3Dコライダーの衝突関数を管理する
	 */
	class RaySystem
	{
		using CollisionFunc = std::function<bool(const Ray&, float, ColliderBase*, RaycastHit*)>;

	private:

		/** @brief 3DコライダーのIDをキーとした衝突関数マップ */
		inline static std::unordered_map<Component::TypeId, CollisionFunc> m_collisionMap;

	public:

		/// @name ライフサイクル
		/// @{

		RaySystem() = default;

		~RaySystem() = default;

		/// @}

		/**
		 * @brief 関数を登録する
		 * @param id コライダーのID
		 * @param func 衝突判定関数
		 * @return 成功したかどうか
		 */
		static bool Register(Component::TypeId id, CollisionFunc func);

		/**
		 * @brief 衝突判定を行う
		 * @param ray 対象のray
		 * @param max Rayが届く最大の距離
		 * @param pCollider コライダー
		 * @param pRaycast 衝突情報を書き込む変数
		 * @return 衝突するかどうか
		 */
		static bool CheckHit(const Ray& ray, float max, ColliderBase* pCollider, RaycastHit* pRaycast);

		/**
		 * @brief Rayと全てのコライダーとの衝突を調べる 最も早くぶつかった衝突情報を調べる
		 * @param colliders コライダーリスト
		 * @param ray 調べるレイ
		 * @param maxDistance 例が届く最大の距離
		 * @param hit 衝突情報を書き込む変数
		 * @param layerMask 衝突するレイヤー
		 * @return 1つでも衝突したかどうか
		 */
		static bool RayCast(const std::vector<ColliderBase*>& colliders, const Ray& ray, float maxDistance, RaycastHit* hit, uint64_t layerMask = 0xFFFFFFFFFFFFFFFF);
	};
}	// namespace RHEngine
