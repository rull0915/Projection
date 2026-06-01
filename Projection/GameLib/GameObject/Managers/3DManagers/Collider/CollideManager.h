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

#include <unordered_set>

// 衝突判定
#include "CheckHit/CheckHit.h"
#include "SpaceDivision/TreeManager.h"
#include "../../HitContact.h"

// Ray関連
#include "../Ray/CheckHitWithRay.h"

//====================================================//
// 前方宣言
//====================================================//


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
    std::vector<BaseCollider*> m_reserves;
    std::unordered_set<BaseCollider*> m_removeReserves;

    // 管理しているコライダー
    std::vector<BaseCollider*> m_colliders;
    std::vector<ObjectForTree*> m_treeObjects;

    // レイヤー管理
    std::vector<std::vector<bool>> m_layer;

    // 木構造
    TreeManager m_tree;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    CollideManager() 
        : m_reserves{}
        , m_removeReserves{}
        , m_colliders{}
        , m_treeObjects{}
        , m_layer{}
        , m_tree{ { 256, 256, 256 }, 5, {0, 0, 0} }
    {
        m_layer.resize(100);
        for (auto& col : m_layer) col.resize(100);
    };

    ~CollideManager() = default;

    // コライダーの追加
    void AddCollide(BaseCollider* collide)
    {
        m_reserves.push_back(collide);
    }

    // コライダーの削除
    void RemoveCollide(BaseCollider* collide)
    {
        // 削除予約リストに追加
        m_removeReserves.insert(collide);
    }

    /// <summary>
    /// レイヤー同士の衝突の有無を管理する関数
    /// </summary>
    void SetCollideActive(int layerA, int layerB, bool isActive)
    {
        // 双方の条件を変更
        m_layer[layerA][layerB] = isActive;
        m_layer[layerB][layerA] = isActive;
    };

    // 登録予約済みのコライダーを追加する関数
    void AddReserved();

    // 削除予約済みのコライダーを削除する関数
    void RemoveReserved();

    // 全コライダーのキャッシュ更新
    void UpdateCaches();

    // 全コライダーの木構造空間での移動
    void MoveAllColliderOnTree();

    // 全コライダーの衝突チェック
    void CheckHitAll(std::vector<HitContact>& contacts);

    // コライダーの衝突チェック
    bool CheckHitPair(BaseCollider*, BaseCollider*, HitContact& contact);

    // レイの衝突判定
    bool RayCast(const Ray& ray, float maxDistance, RaycastHit& hit, uint64_t layerMask = 0xFFFFFFFFFFFFFFFF);
};