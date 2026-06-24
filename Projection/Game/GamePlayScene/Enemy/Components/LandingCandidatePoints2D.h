//====================================================//
// ファイル名   : LandingCandidatePoints2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/14
//
// 概要 : 2Dの着地候補点コンポーネント
//       敵AIに使用する着地候補点を示すコンポーネントです
//
// 更新履歴 :
// 2026/06/14 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/GameObject/Components/Component.h"
#include "GameLib/GameObject/Components/Collider/2D/BaseCollider2D.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class LandingCandidatePoints2D : public Component<LandingCandidatePoints2D, ComponentID::LandingCandidatePoints2D>
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------
    static constexpr float CONVEX_UP_BORDER = 1.0f / 1.41421356;

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 着地候補点
    std::vector<DirectX::SimpleMath::Vector2> m_candidatePoints;

    // 中心点
    DirectX::SimpleMath::Vector2 m_centerPoint;

    // 自身と対応するコライダー
    BaseCollider2D* m_ownCollider;

    // バージョン
    uint32_t m_latestVersion;

    // 変更済みフラグ
    bool m_isChanged;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    LandingCandidatePoints2D(IComponentOwner* owner)
        : Component(owner)
        , m_candidatePoints(0)
        , m_centerPoint{}
        , m_ownCollider{ nullptr }
        , m_latestVersion{ 0 }
        , m_isChanged{ true }
    {
    }

    ~LandingCandidatePoints2D() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Awake() override;

    void Start() override;

    void Update(const GameTimer& gameTimer) override;

    BaseCollider2D* GetOwnCollider() const { return m_ownCollider; }

    // 候補点をすべて取得する関数
    const std::vector<DirectX::SimpleMath::Vector2>& GetPoints() const { return m_candidatePoints; }

    // 中心座標を返す関数
    const DirectX::SimpleMath::Vector2& GetCenterPoint() const { return m_centerPoint; }

    // フラグリセット
    void ResetChangedFlag() { m_isChanged = false; }

    // フラグを返す
    bool IsChanged() const { return m_isChanged; }

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

    // 候補点の座標を作成する関数
    void UpdateCandidatePoints();

    // ボックスコライダーの場合
    void UpdateCandidatePointsOnBox();

    // 凸ポリゴンの場合
    void UpdateCandidatePointsOnConvexPolygon();
};
