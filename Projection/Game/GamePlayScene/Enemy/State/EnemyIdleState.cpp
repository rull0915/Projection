//====================================================//
// ファイル名  : EnemyIdleState.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/23
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "EnemyIdleState.h"

#include "../Enemy.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void EnemyIdleState::Enter()
{}

void EnemyIdleState::Update(const GameTimer& timer)
{
	// 道が更新されていたらMoveに移行
	if (GetOwner()->NeedUpdateWay())
	{
		// パスを更新
		GetOwner()->ToNextPath();

		// ジャンプへ
		RequestChangeState(EnemyStateID::Move);
	}
}

void EnemyIdleState::Exit()
{}
