//====================================================//
// ファイル名   : ChangeColliderComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/02
//
// 概要 : コライダーの切り替えを行うコンポーネント
//
// 更新履歴 :
// 2026/06/02 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/GameObject/Components/Component.h"

#include "GameLib/GameObject/Components/Collider/2D/Shapes/2DColliders.h"
#include "GameLib/GameObject/Components/Collider/3D/Shapes/Colliders.h"

#include "GameLib/GameObject/Components/Camera/BaseCamera.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ChangeColliderComponent : public Component<ChangeColliderComponent, ComponentID::ChangeColliderComponent>
{
private:

    // 3Dコライダーを受け取る配列
    std::vector<BaseCollider*> m_3dColliders;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    ChangeColliderComponent(IComponentOwner* owner)
        : Component(owner)
    {
    }
    ~ChangeColliderComponent() {};

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

	// 3次元から2次元へ切り替える関数
	void Change3DTo2D(BaseCamera* pCamera);

	// 2次元から3次元へ切り替える関数
	void Change2DTo3D();

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------

    // 3次元コライダーから2次元コライダーを生成する関数
	BaseCollider2D* Create2DColliderFrom3D(BaseCamera* pCamera, BaseCollider* p3DCol);
};
