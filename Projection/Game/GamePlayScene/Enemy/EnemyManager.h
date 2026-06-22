//====================================================//
// ファイル名   : EnemyManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要 : 敵管理クラス
//
// 更新履歴 :
// 2026/06/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "AI/NavigationGraph.h"

#include "Enemy.h"
#include "GameLib/GameObject/Components/Transform/Transform.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class EnemyManager
{
    // グラフを更新する感覚
    static constexpr float GRAPH_UPDATE_DISTANCE = 0.2f;

    // 道を再計算するボーダー(距離の2乗)
    static constexpr float WAY_UPDATE_BORDER = 4.0f;
    
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 通常の敵が使用するグラフ
    NavigationGraph m_normalNavigation;

    // 管理している敵リスト
    std::vector<Enemy*> m_enemies;
    std::vector<Enemy*> m_addReserves;
    std::vector<Enemy*> m_removeReserves;

    // 時間管理
    float m_nowTime;

    // プレイヤーのトランスフォーム
    Transform* m_playerTransform;

    // プレイヤーがいた位置
    DirectX::SimpleMath::Vector3 m_oldPlayerPosition;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    EnemyManager()
        : m_normalNavigation{ Enemy::JUMP_IMPLUSE, 1.0f, Enemy::VELOCITY }
        , m_enemies{}
        , m_nowTime{ 0 }
        , m_playerTransform{ nullptr }
    {
    }
    ~EnemyManager() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------
    void Initialize();

    void Update(const GameTimer& timer);

    // 敵を追加する関数
    void AddEnemy(Enemy* component)
    {
        m_addReserves.push_back(component);
    }
    // 敵を削除する関数
    void RemoveEnemy(Enemy* component)
    {
        m_removeReserves.push_back(component);
    }

    // 予約候補点を追加する関数
    void AddPoints(LandingCandidatePoints* component)
    {
        m_normalNavigation.AddNode(component);
    }
    // 予約候補点を削除する関数
    void RemovePoints(LandingCandidatePoints* component)
    {
        m_normalNavigation.RemoveNode(component);
    }

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------


    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------
    void SetPlayer(Transform* player) { m_playerTransform = player; }

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

    // 追加予約済みの敵を追加する
    void AddReserved()
    {
        for (auto& component : m_addReserves)
        {
            m_enemies.push_back(component);
        }

        // リセット
        m_addReserves.clear();
    }

    // 削除予約済みの敵を削除する
    void RemoveReserved()
    {
        for (auto& component : m_removeReserves)
        {
            // 配列にあれば削除する
            m_enemies.erase(
                std::remove(m_enemies.begin(), m_enemies.end(), component),
                m_enemies.end()
            );
        }

        // リセット
        m_removeReserves.clear();
    }
};