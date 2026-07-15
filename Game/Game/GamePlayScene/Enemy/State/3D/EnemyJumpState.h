//====================================================//
// ファイル名   : EnemyJumpState.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/23
//
// 概要 : 敵のジャンプステート
//
// 更新履歴 :
// 2026/06/23 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../EnemyStateBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class EnemyJumpState : public EnemyStateBase
{
	//-----------------------------------------------------
	// メンバ変数宣言
	//-----------------------------------------------------
private:

	// 目標地点
	DirectX::SimpleMath::Vector3 m_targetPosition;

	// 移動するベクトル
	DirectX::SimpleMath::Vector3 m_moveVec;
	
	// 移動量
	float m_moveSpeed;

	// 到着フラグ
	bool m_arrivedXZ;
	bool m_arrivedY;

	//-----------------------------------------------------
	// 関数宣言
	//-----------------------------------------------------

	// コンストラクタ
public:
	EnemyJumpState(Enemy* owner)
		: EnemyStateBase(owner)
		, m_targetPosition{ 0, 0, 0 }
		, m_moveVec{ 0, 0, 0 }
		, m_moveSpeed{ 0 }
		, m_arrivedXZ{ 0 }
		, m_arrivedY{ 0 }
	{}

	// Stateの純粋仮想関数の実装
public:
	void Enter() override;

	void Update(const REngine::GameTimer& timer) override;

	void Exit() override;
};
