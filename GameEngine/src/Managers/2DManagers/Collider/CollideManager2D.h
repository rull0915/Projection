//====================================================//
// ファイル名   : CollideManager2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/16
//
// 概要 : 衝突を管理するクラスです シングルトンで作成します
//
// 更新履歴 :
// 2026/04/16 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "../../ReserveContainer.h"
#include "Components/World/Collider/2D/ColliderBase2D.h"
#include "SpaceDivision/TreeManager2D.h"
#include "Physics/HitContact.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class CollideManager2D : public ReserveContainer<ColliderBase2D>
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 管理しているコライダー
		std::vector<ObjectForTree2D*> m_treeObjects;

		// 衝突リスト保持用配列
		unsigned int m_colCount;
		std::vector<ColliderBase2D*> m_collideList;

		// 木構造
		TreeManager2D m_tree;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		CollideManager2D()
			: m_treeObjects{}
			, m_collideList{}
			, m_tree{ { 256, 256 }, 5, {0, 0} }
			, m_colCount{ 0 }
		{};
		~CollideManager2D() = default;

		// 全コライダーのキャッシュ更新
		void UpdateCaches();

		// 全コライダーの木構造空間での移動
		void MoveAllColliderOnTree();

		// 全コライダーの衝突チェック
		void CheckHitAll(std::vector<HitContact2D>& contacts);

		// コライダーの衝突チェック
		bool CheckHitPair(ColliderBase2D*, ColliderBase2D*, HitContact2D& contact);

		// 全コライダーの取得関数
		const std::vector<ColliderBase2D*>& GetAllColliders()
		{
			// 予約を反映
			AddReserved();
			RemoveReserved();

			return GetObjects();
		}

	private:

		// 登録予約済みのコライダーを追加する関数
		void AddReserved() override;

		// 削除予約済みのコライダーを削除する関数
		void RemoveReserved() override;
	};
}	// namespace REngine
