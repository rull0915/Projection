//====================================================//
// ファイル名   : CollideManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/19
//
// 概要 : 衝突を管理するクラス
//
// 更新履歴 :
// 2026/03/19 新規作成
// 2026/05/04 シングルトンから通常のクラスへ変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <vector>
#include <unordered_set>

// 衝突判定
#include "SpaceDivision/TreeManager.h"
#include "Physics/HitContact.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//

	class CollideManager
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 登録予約中のCollider
		std::vector<ColliderBase*> m_addReserves;
		std::unordered_set<ColliderBase*> m_removeReserves;

		// 管理しているコライダー
		std::vector<ColliderBase*> m_colliders;
		std::vector<ObjectForTree*> m_treeObjects;

		// 木構造
		TreeManager m_tree;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		CollideManager()
			: m_addReserves{}
			, m_removeReserves{}
			, m_colliders{}
			, m_treeObjects{}
			, m_tree{ { 256, 256, 256 }, 5, {0, 0, 0} }
		{};

		~CollideManager() = default;

		// コライダーの追加
		void AddCollide(ColliderBase* collide)
		{
			m_addReserves.push_back(collide);
		}

		// コライダーの削除
		void RemoveCollide(ColliderBase* collide)
		{
			// 削除予約リストに追加
			m_removeReserves.insert(collide);
		}

		// 予約の反映
		void ReflectReserves()
		{
			AddReserved();
			RemoveReserved();
		}

		// 全コライダーのキャッシュ更新
		void UpdateCaches();

		// 全コライダーの木構造空間での移動
		void MoveAllColliderOnTree();

		// 全コライダーの衝突チェック
		void CheckHitAll(std::vector<HitContact>& contacts);

		// コライダーの衝突チェック
		bool CheckHitPair(ColliderBase*, ColliderBase*, HitContact& contact);

		// 所有しているコライダーを全て返す関数
		const std::vector<ColliderBase*> GetAllColliders()
		{
			// 予約を反映
			AddReserved();
			RemoveReserved();

			return m_colliders;
		}
	private:

		// 登録予約済みのコライダーを追加する関数
		void AddReserved();

		// 削除予約済みのコライダーを削除する関数
		void RemoveReserved();
	};
}	// namespace REngine
