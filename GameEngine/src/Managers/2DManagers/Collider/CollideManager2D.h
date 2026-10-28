/**
 * @file    CollideManager2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/16
 *
 * @brief   衝突を管理するクラス
 */

#pragma once

#include <vector>

#include "../../ReserveContainer.h"
#include "Components/World/Collider/2D/ColliderBase2D.h"
#include "SpaceDivision/TreeManager2D.h"
#include "Physics/HitContact.h"
#include "SpaceDivision/ObjectForTree2D.h"

namespace RHEngine
{
	/**
	 * @brief 2Dの衝突を管理する
	 */
	class CollideManager2D : public ReserveContainer<ColliderBase2D>
	{
	private:

		/** @brief 4分木登録用オブジェクトリスト */
		std::vector<ObjectForTree2D*> m_treeObjects;

		/** @brief 衝突したコライダーペアの数 */
		unsigned int m_colCount;

		/** @brief 衝突したコライダーのリスト */
		std::vector<ColliderBase2D*> m_collideList;

		/** @brief 4分木管理クラス */
		TreeManager2D m_tree;

	public:

		/// @name ライフサイクル
		/// @{

		CollideManager2D()
			: m_treeObjects{}
			, m_collideList{}
			, m_tree{ { 256, 256 }, 5, {0, 0} }
			, m_colCount{ 0 }
		{};

		~CollideManager2D() = default;

		/// @}

		/**
		 * @brief 全コライダーのキャッシュを更新する
		 */
		void UpdateCaches();

		/**
		 * @brief 全コライダーのうち動いたコライダーを4分木に再配置する
		 */
		void MoveAllColliderOnTree();

		/**
		 * @brief 全コライダーの衝突チェック
		 * @param contacts 衝突ペアリスト
		 */
		void CheckHitAll(std::vector<HitContact2D>& contacts);

		/**
		 * @brief ブロードフェーズを通過したコライダーのペアに対してナローフェーズを行う
		 * @param contact 衝突情報を書き込む変数
		 * @return 衝突したかどうか
		 */
		bool CheckHitPair(ColliderBase2D*, ColliderBase2D*, HitContact2D& contact);

	private:

		/**
		 * @brief 追加予約を反映する
		 */
		void AddReserved() override;

		/**
		 * @brief 削除予約を反映する
		 */
		void RemoveReserved() override;
	};
}	// namespace RHEngine
