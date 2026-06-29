//====================================================//
// ファイル名   : StateMachine.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/06
//
// 概要 : ステートマシンクラス
//
// 更新履歴 :
// 2026/03/06 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <string>
#include <type_traits>
#include <memory>
#include <unordered_map>

#include "StateBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
class StateMachine
{
	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

private:

	// ステートの配列
	// 列挙子をキー ステートを要素としたmap
	std::unordered_map<T, std::unique_ptr<StateBase<T>>> m_states;

	// 現在のステート
	StateBase<T>* m_currentState;
	T m_currentStateType;

	// 変更要請
	bool m_isRequested;
	T m_nextType;

	//-----------------------------------------------------
	// 関数宣言
	//-----------------------------------------------------

public:

	// コンストラクタ
	StateMachine()
		: m_states{}
		, m_currentState{ nullptr }
		, m_currentStateType{}
		, m_nextType{}
		, m_isRequested{ false }
	{
	}

	// デストラクタ
	~StateMachine() = default;

	// ステートを登録する関数
	bool RegisterState(T key, std::unique_ptr<StateBase<T>> state)
	{
		// 追加可能か調べる
		auto [it, f] = m_states.try_emplace(key, std::move(state));

		// 追加できたかどうかを返す
		return f;
	}

	// 更新処理
	void Update(const GameTimer& timer)
	{
		// 現在のステートが未設定なら何もしない
		if (m_currentState != nullptr) 
		{
			// 現在のステートの更新処理
			m_currentState->Update(timer);
		}

		// ステート変更を確認
		CheckChangeRequset();
	}

	// 現在のステートを返す関数
	const StateBase<T>* GetCurrentState() const
	{
		return m_currentState;
	}

	// 現在のステートタイプを返す関数
	const T GetCurrentStateType() const 
	{
		return m_currentStateType;
	}

	// 外部からステートの変更を要請する関数
	void RequsetChangeState(T type)
	{
		m_isRequested = true;

		m_nextType = type;
	}

	// ステート遷移リクエストを調べる
private:
	void CheckChangeRequset()
	{
		// ステートからの要請を優先する

		// リクエストがあれば
		if (m_currentState && m_currentState->IsRequestedChangeState())
		{
			// ステートを変更する
			ChangeState(m_currentState->GetNextState());

			// フラグのリセット
			m_currentState->ResetRequest();
		}
		else if (m_isRequested)
		{
			// ステートを変更する
			ChangeState(m_nextType);

			// フラグのリセット
			m_isRequested = false;
		}
	}

	// ステートの変更を行う関数
	bool ChangeState(T key)
	{
		// 登録されていないキーなら
		if (!m_states.contains(key))
		{
			// falseを返す
			return false;
		}

		// 終了処理を呼び出す
		if(m_currentState) m_currentState->Exit();

		// ステートを変更する
		m_currentState = m_states[key].get();

		m_currentStateType = key;

		// 開始処理を呼び出す
		if(m_currentState) m_currentState->Enter();

		return true;
	}
};
