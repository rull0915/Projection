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

#include "GameLib/GameObject/Components/Collider/2D/BaseCollider2D.h"

#include "SpaceDivision/TreeManager2D.h"

#include "../../HitContact.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class CollideManager2D
{
private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 登録予約中のCollider
    std::vector<BaseCollider2D*> m_reserves;
    std::unordered_set<BaseCollider2D*> m_removeReserves;

    // 管理しているコライダー
    std::vector<BaseCollider2D*> m_colliders;
    std::vector<ObjectForTree2D*> m_treeObjects;

    // 衝突リスト保持用配列
    unsigned int m_colCount;
    std::vector<BaseCollider2D*> m_collideList;

    // 木構造
    TreeManager2D m_tree;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    CollideManager2D() 
        : m_reserves{}
        , m_removeReserves{}
        , m_colliders{}
        , m_treeObjects{}
        , m_collideList{}
        , m_tree{ { 256, 256 }, 5, {0, 0} }
        , m_colCount{ 0 }
    {
    };
    ~CollideManager2D() = default;

    // コライダーの追加
    void AddCollide(BaseCollider2D* collide)
    {
        m_reserves.push_back(collide);
    }

    // コライダーの削除
    void RemoveCollide(BaseCollider2D* collide)
    {
        m_removeReserves.insert(collide);
    }

    // 登録予約済みのコライダーを追加する関数
    void AddReserved();

    // 削除予約済みのコライダーを削除する関数
    void RemoveReserved();

    // 全コライダーのキャッシュ更新
    void UpdateCaches();

    // 全コライダーの木構造空間での移動
    void MoveAllColliderOnTree();

    // 全コライダーの衝突チェック
    void CheckHitAll(std::vector<HitContact2D>& contacts);

    // コライダーの衝突チェック
    bool CheckHitPair(BaseCollider2D*, BaseCollider2D*, HitContact2D& contact);
};