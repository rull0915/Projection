/**
 * @file    ChangeColliderComponent.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/02
 *
 * @brief   3Dと2Dのコライダーの切り替えを行う
 */
#pragma once

#include "Components/World/WorldComponentBase.h"

#include "Components/World/Collider/2D/Shapes/2DColliders.h"
#include "Components/World/Collider/3D/Shapes/Colliders.h"

#include "Components/World/Camera/CameraBase.h"

/**
 * @brief 3Dと2Dのコライダーを切り替える
 * @details 3Dから2Dへの切り替え時に3D形状を2D世界に投影することで見た目通りの衝突形状を生成する
 */
class ChangeColliderComponent : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

public:

	ChangeColliderComponent(RHEngine::IComponentOwner* owner)
		: WorldComponentBase(owner)
	{
	}
	~ChangeColliderComponent() {};

	/**
	 * @brief 3次元から2次元に切り替える
	 * @param pCamera 現在のカメラ
	 */
	void Change3DTo2D(RHEngine::CameraBase* pCamera);

	/**
	 * @brief 2次元から3次元に切り替える
	 */
	void Change2DTo3D();

	/**
	 * @brief 2Dコライダーを再構築する
	 * @param pCamera 現在のカメラ
	 */
	void Update2DCollider(RHEngine::CameraBase* pCamera)
	{
		// 再構築
		Change2DTo3D();
		Change3DTo2D(pCamera);
	}

private:

	/**
	 * @brief 3Dコライダーから2Dコライダーを生成する
	 * @param pCamera 現在のカメラ
	 * @param p3DCol 元となる3Dコライダー
	 * @return 生成した2Dコライダー
	 */
	RHEngine::ColliderBase2D* Create2DColliderFrom3D(RHEngine::CameraBase* pCamera, RHEngine::ColliderBase* p3DCol);
};
