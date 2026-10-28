/**
 * @file    Player.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/29
 *
 * @brief   プレイヤークラス
 */
#pragma once

#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

/**
 * @brief プレイヤーコンポーネント
 */
class Player : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:

	static constexpr float MOVE_SPEED = 5.0f;   // 移動の速度
	static constexpr float JUMP_POWER = 15.0f;  // ジャンプの力
	static constexpr float CAN_JUMP_BORDER = 1.0f / 1.414f; // ジャンプ可能な地面の傾きのボーダー(cos45°の近似値をボーダーに)

	RHEngine::Transform* m_pTransform;

	// 現在の次元が2次元かどうか
	bool m_is2D;

	// ジャンプ可能フラグ
	bool m_canJump;

public:

	Player(RHEngine::IComponentOwner* owner);
	~Player();

	void Start() override;
	void Update(const RHEngine::GameTimer& gameTimer) override;

	void OnTriggerStay(RHEngine::HitContact& contact) override;
	void OnTriggerExit(RHEngine::HitContact& contact) override;

	// ジャンプ可能かどうか
	bool CanJump() const { return m_canJump; }

	// 次元フラグを変更する関数
	void ChangeDimention() { m_is2D = !m_is2D; }

	// 2Dかどうか
	bool Is2D() const { return m_is2D; }

private:

	// 2D時の更新関数
	void Update2D(const RHEngine::GameTimer& timer);

	// 3D時の更新関数
	void Update3D(const RHEngine::GameTimer& timer);
};
