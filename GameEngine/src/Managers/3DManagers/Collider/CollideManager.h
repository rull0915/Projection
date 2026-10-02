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

// 衝突判定
#include "SpaceDivision/TreeManager.h"
#include "Physics/HitContact.h"
#include "../../ReserveContainer.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//

	class CollideManager : public ReserveContainer<ColliderBase>
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 管理しているコライダー
		std::vector<ObjectForTree*> m_treeObjects;

		// 木構造
		TreeManager m_tree;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		CollideManager()
			: m_treeObjects{}
			, m_tree{ { 1024, 1024, 1024 }, 5, {0, 0, 0} }
		{};

		~CollideManager() = default;

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

			return GetObjects();
		}
	private:

		// 登録予約済みのコライダーを追加する関数
		void AddReserved() override;

		// 削除予約済みのコライダーを削除する関数
		void RemoveReserved() override;
	};
}	// namespace REngine
