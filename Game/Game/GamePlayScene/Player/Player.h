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

#include "Components/World/Collider/PhysicsMaterial.h"

#include "../Enemy/Components/LandingCandidatePoints.h"
#include "../Enemy/Components/LandingCandidatePoints2D.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class Player : public WorldComponentBase
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
	Transform* m_pTransform;

	PhysicsMaterial m_material;

	// 現在の次元が2次元かどうか
	bool m_is2D;

	// ジャンプ可能フラグ
	bool m_canJump;

	// 最後に触れた候補点
	LandingCandidatePoints* m_lastPoints;
	LandingCandidatePoints2D* m_lastPoints2d;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------

	Player(IComponentOwner* owner);
	~Player();

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;
	void Update(const GameTimer& gameTimer) override;

	void OnCollisionEnter(HitContact& contact) override;
	void OnCollisionEnter2D(HitContact2D& contact) override;

	void OnTriggerStay(HitContact& contact) override;
	void OnTriggerExit(HitContact& contact) override;

	// ジャンプ可能かどうか
	bool CanJump() const { return m_canJump; }

	// 次元フラグを変更する関数
	void ChangeDimention() { m_is2D = !m_is2D; }

	// 最後に触れた候補点を取得する関数
	LandingCandidatePoints* GetLandingPoints() const { return m_lastPoints; }
	LandingCandidatePoints2D* GetLandingPoints2D() const { return m_lastPoints2d; }

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<Player>();
	}

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 2D時の更新関数
	void Update2D(const GameTimer& timer);

	// 3D時の更新関数
	void Update3D(const GameTimer& timer);
};
