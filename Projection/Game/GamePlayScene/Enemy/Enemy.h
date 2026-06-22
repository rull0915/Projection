//====================================================//
// ファイル名   : Enemy.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要 : 敵コンポーネント
//
// 更新履歴 :
// 2026/06/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/GameObject/Components/Component.h"
#include "GameLib/GameObject/Components/Transform/Transform.h"

#include "Components/LandingCandidatePoints.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class Enemy : public Component<Enemy, ComponentID::Enemy>
{
public:
    //-----------------------------------------------------
    // constexpr宣言
    //-----------------------------------------------------
    static constexpr float VELOCITY = 5.0f;
    static constexpr float JUMP_IMPLUSE = 15.0f;

public:
    //-----------------------------------------------------
    // 構造体
    //-----------------------------------------------------
    struct Path
    {
        DirectX::SimpleMath::Vector3 start;     // 始点
        DirectX::SimpleMath::Vector3 goal;      // 終点

        float time; // 移動にかかる時間
    };

private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // トランスフォームのポインタ
    Transform* m_pTransform;   
    
    // 経路
    std::vector<Path> m_way;

    // 辿っている道のインデックス
    size_t m_nowIndex;

    // 辿っている道
    Path m_nowPath;

    // 最後に着地したポイント
    LandingCandidatePoints* m_lastPoints;

    // インターバル中かどうか
    bool m_isInterval;

    // 速度

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    Enemy(IComponentOwner* owner)
        : Component(owner)
        , m_pTransform{ nullptr }
        , m_way{}
        , m_nowIndex{ 0 }
        , m_lastPoints{ nullptr }
    {
    }

    ~Enemy();

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Awake() override;

    void Start() override;

    void Update(const GameTimer& gameTimer) override;

    void OnCollisionEnter(HitContact& hit) override;

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

    LandingCandidatePoints* GetLandingPoints() const { return m_lastPoints; }

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

    void SetWay(const std::vector<Path>& way) 
    {
        m_way = way; 

        m_nowIndex = 0;
    }

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

};
