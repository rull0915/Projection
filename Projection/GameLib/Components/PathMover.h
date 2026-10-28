/**
 * @file    PathMover.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   パスを使用し移動するコンポーネント
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "CatmullRomComponent.h"
#include "Components/World/Transform/Transform.h"

/**
 * @brief パスを使って動かす
 */
class PathMover : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

	enum class RotType
	{
		None,
		Forward,
		Rigit,
		Left
	};

private:

	// 移動する倍率
	float m_magnification;

	// 開始時に移動開始するかどうか
	bool m_playOnStart;

	// 移動中フラグ
	bool m_isPlaying;

	// 合計経過時間
	float m_sumTime;

	// トランスフォーム
	RHEngine::Transform* m_pTransform;

	// 補間コンポーネント
	CatmullRomComponent* m_pCatmull;

	// 回転の補正方法
	RotType m_rotateType;

public:

	PathMover(RHEngine::IComponentOwner* own);
	~PathMover() = default;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
	void LateUpdate(const RHEngine::GameTimer& gameTimer) override;

	// 移動開始関数
	void MoveStart();
};
