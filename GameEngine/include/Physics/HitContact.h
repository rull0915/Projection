/**
 * @file    HitContact.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/24
 *
 * @brief   衝突情報をまとめた構造体
 */

#pragma once

#include <SimpleMath.h>

namespace RHEngine
{
	class IComponentOwner;
	class ColliderBase;
	class ColliderBase2D;
	class RigidBody;
	class RigidBody2D;

	/**
	 * @brief 2つのコライダーのペア
	 */
	struct ObjectPair
	{
		ColliderBase* a;
		ColliderBase* b;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pA 1つのコライダー
		 * @param pB もう一方のコライダー
		 */
		ObjectPair(ColliderBase* pA, ColliderBase* pB)
		{
			// 順序を保証する
			a = pA > pB ? pA : pB;
			b = pA < pB ? pA : pB;
		}

		/// @}

		/**
		 * @brief 等価演算子オーバーロード
		 * @param other 比較対象
		 */
		bool operator==(const ObjectPair& other) const
		{
			// 順不同で2つが一致するか調べる
			return (a == other.a && b == other.b) ||
				(a == other.b && b == other.a);
		}
	};

	/**
	 * @brief ObjectPair用のハッシュ構造体
	 */
	struct ObjectPairHash
	{
		size_t operator()(const ObjectPair& p) const
		{
			size_t h1 = std::hash<ColliderBase*>()(p.a);
			size_t h2 = std::hash<ColliderBase*>()(p.b);
			return h1 ^ (h2 << 1);
		}
	};

	/**
	 * @brief 2Dコライダーのペア
	 */
	struct ObjectPair2D
	{
		ColliderBase2D* a;
		ColliderBase2D* b;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pA 1つのコライダー
		 * @param pB もう一方のコライダー
		 */
		ObjectPair2D(ColliderBase2D* pA, ColliderBase2D* pB)
		{
			// 順序を保証する
			a = pA > pB ? pA : pB;
			b = pA < pB ? pA : pB;
		}

		/// @}

		/**
		 * @brief 等価演算子オーバーロード
		 * @param other 比較対象
		 */
		bool operator==(const ObjectPair2D& other) const
		{
			// 順不同で2つが一致するか調べる
			return (a == other.a && b == other.b) ||
				(a == other.b && b == other.a);
		}
	};

	/**
	 * @brief ObjectPair2D用のハッシュ構造体
	 */
	struct ObjectPairHash2D
	{
		size_t operator()(const ObjectPair2D& p) const
		{
			size_t h1 = std::hash<ColliderBase2D*>()(p.a);
			size_t h2 = std::hash<ColliderBase2D*>()(p.b);
			return h1 ^ (h2 << 1);
		}
	};

	/**
	 * @brief 3D衝突時の情報
	 */
	struct HitContact
	{
		IComponentOwner* own = nullptr;		///< 自身の所有者
		IComponentOwner* other = nullptr;	///< 衝突相手の所有者

		RigidBody* ownRigid = nullptr;		///< 自身のRigidBody
		RigidBody* otherRigid = nullptr;	///< 衝突相手のRigidBody

		ColliderBase* ownCol = nullptr;		///< 自身のコライダー
		ColliderBase* otherCol = nullptr;	///< 衝突相手のコライダー

		bool isTrigger = false;		///< トリガーの衝突かどうか

		bool ownIsStatic = false;	///< 自身が固定かどうか
		bool otherIsStatic = false;	///< 衝突相手が固定かどうか

		DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;	///< 衝突法線

		float penetration = 0.0f;	///< めり込み量

		/**
		 * @brief 衝突点 (未使用)
		 * @TODO 衝突点を計算し、回転込みの物理挙動を扱えるようにする
		 */
		DirectX::SimpleMath::Vector3 point = DirectX::SimpleMath::Vector3::Zero;

		/**
		 * @brief 衝突相手から見た衝突情報を作成する
		 * @return 作成した衝突情報
		 */
		HitContact Inverse() const
		{
			HitContact hit;

			hit.own = other;
			hit.other = own;

			hit.ownRigid = otherRigid;
			hit.otherRigid = ownRigid;

			hit.ownCol = otherCol;
			hit.otherCol = ownCol;

			hit.isTrigger = isTrigger;

			hit.ownIsStatic = otherIsStatic;
			hit.otherIsStatic = ownIsStatic;

			hit.normal = -normal;
			hit.penetration = penetration;
			hit.point = point;

			return hit;
		}
	};

	/**
	 * @brief 3D衝突時の情報
	 */
	struct HitContact2D
	{
		IComponentOwner* own = nullptr;		///< 自身の所有者
		IComponentOwner* other = nullptr;	///< 衝突相手の所有者

		RigidBody2D* ownRigid = nullptr;	///< 自身のRigidBody2D
		RigidBody2D* otherRigid = nullptr;	///< 衝突相手のRigidBody2D

		ColliderBase2D* ownCol = nullptr;	///< 自身のコライダー
		ColliderBase2D* otherCol = nullptr;	///< 衝突相手のコライダー

		bool isTrigger = false;		///< トリガーの衝突かどうか

		bool ownIsStatic = false;	///< 自身が固定かどうか
		bool otherIsStatic = false;	///< 衝突相手が固定かどうか

		DirectX::SimpleMath::Vector2 normal = DirectX::SimpleMath::Vector2::Zero;	///< 衝突法線

		float penetration = 0.0f;	///< めり込み量

		/**
		 * @brief 衝突点 (未使用)
		 * @TODO 衝突点を計算し、回転込みの物理挙動を扱えるようにする
		 */
		DirectX::SimpleMath::Vector2 point = DirectX::SimpleMath::Vector2::Zero;

		/**
		 * @brief 衝突相手から見た衝突情報を作成する
		 * @return 作成した衝突情報
		 */
		HitContact2D Inverse() const
		{
			HitContact2D hit;

			hit.own = other;
			hit.other = own;

			hit.ownRigid = otherRigid;
			hit.otherRigid = ownRigid;

			hit.ownCol = otherCol;
			hit.otherCol = ownCol;

			hit.isTrigger = isTrigger;

			hit.ownIsStatic = otherIsStatic;
			hit.otherIsStatic = ownIsStatic;

			hit.normal = -normal;
			hit.penetration = penetration;
			hit.point = point;

			return hit;
		}
	};
}	// namespace RHEngine
