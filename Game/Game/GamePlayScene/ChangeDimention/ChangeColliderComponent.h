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

#define IS_COMPONENT(ChangeColliderComponent)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"

#include "Components/World/Collider/2D/Shapes/2DColliders.h"
#include "Components/World/Collider/3D/Shapes/Colliders.h"

#include "Components/World/Camera/CameraBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ChangeColliderComponent : public WorldComponentBase
{
private:

	// 3Dコライダーを受け取る配列
	std::vector<ColliderBase*> m_3dColliders;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ChangeColliderComponent(IComponentOwner* owner)
		: WorldComponentBase(owner)
		, m_3dColliders( 0 )
	{
		m_3dColliders.clear();
	}
	~ChangeColliderComponent() {};

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 3次元から2次元へ切り替える関数
	void Change3DTo2D(CameraBase* pCamera);

	// 2次元から3次元へ切り替える関数
	void Change2DTo3D();

	// 2次元コライダーを更新する関数
	void Update2DCollider(CameraBase* pCamera)
	{
		// 再構築
		Change2DTo3D();
		Change3DTo2D(pCamera);
	}

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<ChangeColliderComponent>();
	}

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 3次元コライダーから2次元コライダーを生成する関数
	ColliderBase2D* Create2DColliderFrom3D(CameraBase* pCamera, ColliderBase* p3DCol);
};
