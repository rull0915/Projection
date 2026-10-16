//====================================================//
// ファイル名   : GameTimer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/04
//
// 概要 : 各オブジェクトに渡すゲーム用のタイマークラス
//
// 更新履歴 :
// 2026/06/04 新規作成
//====================================================//

#pragma once

namespace RHEngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class GameTimer
	{
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// スケールされた総経過時間
		float m_time;

		// 本当の総経過時間
		float m_unscaledTime;

		// スケールされた経過時間
		float m_elapsedTime;

		// 本当の経過時間
		float m_unscaledElapsedTime;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		GameTimer()
			: m_time{ 0.0f }
			, m_unscaledTime{ 0.0f }
			, m_elapsedTime{ 0.0f }
			, m_unscaledElapsedTime{ 0.0f }
		{}
		~GameTimer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Update(float elapsedTime);

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------
		float GetElapsedTime() const { return m_elapsedTime; }
		float GetUnScaledElapsedTime() const { return m_unscaledElapsedTime; }

		float GetTime() const { return m_time; }
		float GetUnScaledTime() const { return m_unscaledTime; }
	};
}	// namespace RHEngine
