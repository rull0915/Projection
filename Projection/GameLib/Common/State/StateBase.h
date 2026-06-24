//====================================================//
// ファイル名   : BaseState.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/06
//
// 概要 : 各ステートの基底クラス。
//
// 更新履歴 :
// 2026/03/06 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <type_traits>

//====================================================//
// クラス宣言
//====================================================//
template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
class StateBase
{
private:

    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // ステート変更リクエストフラグ
    bool m_requestedChangeState = false;

    // 変更要求ステート
    T m_nextState;

    //-----------------------------------------------------
    // 関数宣言
    //-----------------------------------------------------

    // インターフェースの純粋仮想関数
public:
    virtual void Enter() = 0;   // 最初

    virtual void Update(const GameTimer& timer) = 0;  // 常時   

    virtual void Exit() = 0;    // 最後

    // 公開関数
public:

    StateBase()
        : m_requestedChangeState{ false }
        , m_nextState{}
    {}
    virtual ~StateBase() = default;

    // ステート変更リクエストフラグ
    bool IsRequestedChangeState() const { return m_requestedChangeState; }

    // 次のステート
    T GetNextState() const { return m_nextState; }

    // フラグのリセット
    void ResetRequest() { m_requestedChangeState = false; }

    // ステート変更リクエスト
protected:

    void RequestChangeState(T next)
    {
        // フラグを立てる
        m_requestedChangeState = true;

        // 次のステートを設定
        m_nextState = next;
    }
};
