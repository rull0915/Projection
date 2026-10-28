/**
 * @file    CollideManager.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/19
 *
 * @brief   衝突を管理するクラス
 */

#pragma once

#include <vector>

#include "SpaceDivision/TreeManager.h"
#include "Physics/HitContact.h"
#include "../../ReserveContainer.h"
#include "Components/World/Collider/3D/ColliderBase.h"
#include "SpaceDivision/ObjectForTree.h"

namespace RHEngine
{
	/**
	 * @brief 衝突判定管理クラス
	 */
	class CollideManager : public ReserveContainer<ColliderBase>
	{
	private:

		/** @brief 管理中のオブジェクトリスト */
		std::vector<ObjectForTree*> m_treeObjects;

		/** @brief 8分木管理クラス */
		TreeManager m_tree;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @TODO マジックナンバーの解消
		 */
		CollideManager()
			: m_treeObjects{}
			, m_tree{ { 1024, 1024, 1024 }, 5, {0, 0, 0} }
		{};

		~CollideManager() = default;

		/// @}

		/**
		 * @brief 全コライダーのキャッシュを更新する
		 */
		void UpdateCaches();

		/**
		 * @brief 全コライダーのうち動いたコライダーを8分木に再配置する
		 */
		void MoveAllColliderOnTree();

		/**
		 * @brief 全コライダーの衝突判定
		 * @param contacts 衝突情報リスト
		 */
		void CheckHitAll(std::vector<HitContact>& contacts);

		/**
		 * @brief ブロードフェーズを通過したコライダーのペアに対してナローフェーズを行う
		 * @param contact 衝突情報を書き込む変数
		 * @return 衝突したかどうか
		 */
		bool CheckHitPair(ColliderBase*, ColliderBase*, HitContact& contact);

	private:

		/**
		 * @brief 登録予約の反映
		 */
		void AddReserved() override;

		/**
		 * @brief 削除予約の反映
		 */
		void RemoveReserved() override;
	};
}	// namespace RHEngine
