//====================================================//
// ファイル名   : SlideTransition.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要 : フェードのシーン遷移
//
// 更新履歴 :
// 2026/06/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Scene/Transition/TransitionBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//

namespace Transition
{
	class Slide : public Base
	{
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------
		enum class End
		{
			RightUp,
			RightDown,
			LeftUp,
			LeftDown,
		};

	private:

		// 経過時間
		float m_elapsedTime;

		float m_transSec;

		int m_slideColor;

		float m_slideAngle;

		// 最終系の4点座標
		DirectX::SimpleMath::Vector2 m_points[4];

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Slide(float transTime, int slideColor = 0x000000, float slideAngle = 0)
			: m_elapsedTime{ 0.0f }
			, m_transSec{ transTime }
			, m_slideColor{ slideColor }
			, m_slideAngle{ slideAngle }
			, m_points{}
		{}
		~Slide() = default;

		// 純粋仮想関数
		void Initialize() override;

		// 更新関数
		// 遷移終了時にtrue
		bool InUpdate(const GameTimer& gameTimer) override;	    // Inの場合	
		bool OutUpdate(const GameTimer& gameTimer) override;	// Outの場合	

		// 描画関数
		void InRender(Renderer& renderer) override;	    // Inの場合
		void OutRender(Renderer& renderer) override;	// Outの場合	
	};
}
