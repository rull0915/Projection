/**
 * @file    CollisionMap.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/26
 *
 * @brief   衝突判定の関数マップを管理する
 */

#pragma once

#include <functional>
#include <unordered_map>

#include "Physics/HitInfomation.h"
#include "Physics/HitInfomation2D.h"
#include "Components/ComponentTypeId.h"

namespace RHEngine
{
	class ColliderBase;
	class ColliderBase2D;

	/**
	 * @brief 2つのIDを持つキー
	 */
	struct CollisionKey
	{
		Component::TypeId type1;
		Component::TypeId type2;
	};

	/**
	 * @brief キーをunordered_mapで使用するためのハッシュ構造体
	 */
	struct CollisionKeyHash
	{
		size_t operator()(const CollisionKey& key) const
		{
			const size_t h1 = std::hash<Component::TypeId>{}(key.type1);
			const size_t h2 = std::hash<Component::TypeId>{}(key.type2);

			return h1 ^ (h2 << 1);
		}
	};

	/**
	 * @brief 3次元衝突の関数マップ
	 */
	class CollisionMap
	{
		using CollisionFunc = std::function<bool(ColliderBase*, ColliderBase*, HitInfomation*)>;

	private:

		/** @brief IDのペアをキーとした衝突関数のマップ */
		inline static std::unordered_map<CollisionKey, CollisionFunc, CollisionKeyHash> m_collisionMap;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @details 静的クラスのためインスタンス化を禁止
		 */
		CollisionMap() = delete;

		~CollisionMap() = default;

		/// @}

		/**
		 * @brief 衝突関数を登録する
		 * @param id1 1つ目のコライダーのID
		 * @param id2 2つ目のコライダーのID
		 * @param func 衝突関数
		 * @return 成功したかどうか
		 */
		static bool Register(Component::TypeId id1, Component::TypeId id2, CollisionFunc func);

		/**
		 * @brief 衝突チェック
		 * @param col1 1つ目のコライダー
		 * @param col2 2つ目のコライダー
		 * @param info 衝突情報を書き込む変数
		 * @return 衝突しているかどうか
		 */
		static bool CheckHit(ColliderBase* col1, ColliderBase* col2, HitInfomation* info);
	};

	// 2次元コライダー衝突マップ
	class CollisionMap2D
	{
		// マクロ宣言
		using CollisionFunc = std::function<bool(ColliderBase2D*, ColliderBase2D*, HitInfomation2D*)>;

	private:

		/** @brief IDのペアをキーとした衝突関数のマップ */
		inline static std::unordered_map<CollisionKey, CollisionFunc, CollisionKeyHash> m_collisionMap;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @details 静的クラスのためインスタンス化を禁止
		 */
		CollisionMap2D() = delete;

		~CollisionMap2D() = default;

		/// @}
		/**
		 * @brief 衝突関数を登録する
		 * @param id1 1つ目のコライダーのID
		 * @param id2 2つ目のコライダーのID
		 * @param func 衝突関数
		 * @return 成功したかどうか
		 */
		static bool Register(Component::TypeId id1, Component::TypeId id2, CollisionFunc func);

		/**
		 * @brief 衝突チェック
		 * @param col1 1つ目のコライダー
		 * @param col2 2つ目のコライダー
		 * @param info 衝突情報を書き込む変数
		 * @return 衝突しているかどうか
		 */
		static bool CheckHit(ColliderBase2D* col1, ColliderBase2D* col2, HitInfomation2D* info);
	};
}	// namespace RHEngine
