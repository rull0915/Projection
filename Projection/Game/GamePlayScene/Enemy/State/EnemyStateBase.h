//====================================================//
// ファイル名   : EnemyStateBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/16
//
// 概要 : 敵ステートの基底クラス
//
// 更新履歴 :
// 2026/06/16 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameLib/Common/State/StateBase.h"

//====================================================//
// 前方宣言
//====================================================//
class Enemy;

//====================================================//
// 列挙型宣言
//====================================================//
enum class EnemyStateID
{
    Idle,
    Move,
    Jump,
};

//====================================================//
// クラス宣言
//====================================================//
class EnemyStateBase : public StateBase<EnemyStateID>
{
private:
    Enemy* m_owner = nullptr;

public:
    EnemyStateBase(Enemy* owner)
        : m_owner{ owner }
    {
    }

    virtual void Enter() override = 0;

    virtual void Update(const GameTimer& timer) override = 0;

    virtual void Exit() override = 0;

    Enemy* GetOwner() const
    {
        return m_owner;
    }
};
