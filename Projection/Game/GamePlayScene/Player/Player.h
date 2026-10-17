//====================================================//
// ファイル名   : Player.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/29
//
// 概要 : プレイヤークラス
//
// 更新履歴 :
// 2026/05/29 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(Player)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class Player : public RHEngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------
	static constexpr float MOVE_SPEED = 5.0f;   // 移動の速度
	static constexpr float JUMP_POWER = 15.0f;  // ジャンプの力
	static constexpr float CAN_JUMP_BORDER = 1.0f / 1.414f; // ジャンプ可能な地面の傾きのボーダー(cos45°の近似値をボーダーに)

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------
	RHEngine::Transform* m_pTransform;

	// 現在の次元が2次元かどうか
	bool m_is2D;

	// ジャンプ可能フラグ
	bool m_canJump;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------

	Player(RHEngine::IComponentOwner* owner);
	~Player();

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------

	COMPONENT_TYPE(Player, RHEngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

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

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 2D時の更新関数
	void Update2D(const RHEngine::GameTimer& timer);

	// 3D時の更新関数
	void Update3D(const RHEngine::GameTimer& timer);
};
