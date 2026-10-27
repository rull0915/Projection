/**
 * @file    BaseState.h
 * @author  Hoshino Ryunosuke
 * @date    2026/03/06
 *
 * @brief   各ステートの基底クラス
 */

#pragma once

#include <type_traits>

namespace RHEngine
{
	/**
	 * @brief ステートの基底
	 * @tparam T ステートの識別子として扱う列挙型
	 */
	template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	class StateBase
	{
	private:

		/** @brief ステート変更リクエストフラグ */
		bool m_requestedChangeState = false;

		/** @brief 変更要求ステート識別子 */
		T m_nextState;

	public:

		/// @name ライフサイクル
		/// @{

		StateBase()
			: m_requestedChangeState{ false }
			, m_nextState{}
		{}

		virtual ~StateBase() = default;

		/// @}

		/// @name 純粋仮想関数
		/// @{

		/**
		 * @brief ステートに入った際に呼ばれる
		 */
		virtual void Enter() = 0; 

		/**
		 * @brief そのステートの間毎フレーム呼ばれる
		 * @param timer タイマー
		 */
		virtual void Update(const GameTimer& timer) = 0; 

		/**
		 * @brief ステート終了時に呼ばれる
		 */
		virtual void Exit() = 0; 

		/// @}

		/**
		 * @brief 変更リクエストの状態を調べる
		 * @return ステート変更がリクエストされているかどうか
		 */
		bool IsRequestedChangeState() const { return m_requestedChangeState; }

		/**
		 * @brief 変更要請先のステート識別子
		 * @return 
		 */
		T GetNextState() const { return m_nextState; }

		/**
		 * @brief リクエストのリセット
		 */
		void ResetRequest() { m_requestedChangeState = false; }

	protected:

		/**
		 * @brief ステートの変更をリクエストする
		 * @param next 変更後ステートの識別子
		 */
		void RequestChangeState(T next)
		{
			// フラグを立てる
			m_requestedChangeState = true;

			// 次のステートを設定
			m_nextState = next;
		}
	};
} // namespace RHEngine
