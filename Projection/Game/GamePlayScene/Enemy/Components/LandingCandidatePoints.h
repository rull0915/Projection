//====================================================//
// ファイル名   : LandingCandidatePoints.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/14
//
// 概要 : 着地候補点コンポーネント
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
#include "GameLib/GameObject/Components/Collider/3D/BaseCollider.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class LandingCandidatePoints : public Component<LandingCandidatePoints, ComponentID::LandingCandidatePoints>
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 着地候補点
    std::vector<DirectX::SimpleMath::Vector3> m_candidatePoints;

    // 自身と対応するコライダー
    BaseCollider* m_ownCollider;

    // バージョン
    uint32_t m_latestVersion;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    LandingCandidatePoints(IComponentOwner* owner)
        : Component(owner)
        , m_ownCollider{ nullptr }
        , m_latestVersion{ 0 }
    {
    }

    ~LandingCandidatePoints();

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Awake() override;

    void Start() override;

    void Update(const GameTimer& gameTimer) override;

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

    // 候補点の座標を作成する関数
    void UpdateCandidatePoints();

    // ボックスコライダーの場合
    void UpdateCandidatePointsOnBox();
};
