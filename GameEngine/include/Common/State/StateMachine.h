/**
 * @file    StateMachine.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/06
 *
 * @brief   ステートマシン
 */

#pragma once

#include <string>
#include <type_traits>
#include <memory>
#include <unordered_map>

#include "StateBase.h"

namespace RHEngine
{
	/**
	 * @brief ステートマシン
	 * @tparam T ステート識別用の列挙型
	 */
	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	class StateMachine
	{
	private:

		/** @brief ステート識別列挙子とステート本体の対応マップ */
		std::unordered_map<T, std::unique_ptr<StateBase<T>>> m_states;

		/** @brief 現在のステート */
		StateBase<T>* m_currentState;

		/** @brief 現在のステート識別子 */
		T m_currentStateType;

		/** @brief ステート変更がリクエストされているかどうか */
		bool m_isRequested;

		/** @brief 変更要請中のステート識別子 */
		T m_nextType;

	public:

		/// @name ライフサイクル
		/// @{

		StateMachine()
			: m_states{}
			, m_currentState{ nullptr }
			, m_currentStateType{}
			, m_nextType{}
			, m_isRequested{ false }
		{}

		~StateMachine() = default;

		/// @}

		/**
		 * @brief ステートを登録する
		 * @param key ステートの識別子
		 * @param state ステート本体
		 * @return 成功したかどうか
		 */
		bool RegisterState(T key, std::unique_ptr<StateBase<T>> state)
		{
			// 追加可能か調べる
			auto [it, f] = m_states.try_emplace(key, std::move(state));

			// 追加できたかどうかを返す
			return f;
		}

		/**
		 * @brief 更新関数
		 * @param timer ゲーム全体で使用するタイマー
		 */
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

		/**
		 * @brief 現在のステートを取得する
		 * @return ステートのポインタ
		 */
		const StateBase<T>* GetCurrentState() const
		{
			return m_currentState;
		}

		/**
		 * @brief 現在のステート識別子を返す
		 * @return ステートの識別子
		 */
		const T GetCurrentStateType() const
		{
			return m_currentStateType;
		}

		/**
		 * @brief ステートの変更をリクエストする
		 * @param type 変更後のステート識別子
		 */
		void RequsetChangeState(T type)
		{
			m_isRequested = true;

			m_nextType = type;
		}

	private:
		/**
		 * @brief ステート変更リクエストを調べ遷移する
		 */
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

		/**
		 * @brief ステートを変更する
		 * @param key 変更対象のステート識別子
		 * @return 成功したかどうか
		 */
		bool ChangeState(T key)
		{
			// 登録されていないキーなら
			if (!m_states.contains(key))
			{
				// falseを返す
				return false;
			}

			// 終了処理を呼び出す
			if (m_currentState) m_currentState->Exit();

			// ステートを変更する
			m_currentState = m_states[key].get();

			m_currentStateType = key;

			// 開始処理を呼び出す
			if (m_currentState) m_currentState->Enter();

			return true;
		}
	};
} // namespace RHEngine
