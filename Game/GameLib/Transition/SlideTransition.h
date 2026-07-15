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
	class Slide : public REngine::Transition::Base
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

		DirectX::SimpleMath::Color m_slideColor;

		float m_slideAngle;

		// 最終系の4点座標
		DirectX::SimpleMath::Vector2 m_points[4];

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Slide(float transTime, DirectX::SimpleMath::Color slideColor = { 0, 0, 0, 1 }, float slideAngle = 0)
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
		bool InUpdate(const REngine::GameTimer& gameTimer) override;	    // Inの場合	
		bool OutUpdate(const REngine::GameTimer& gameTimer) override;	// Outの場合	

		// 描画関数
		void InRender(REngine::Renderer& renderer) override;	    // Inの場合
		void OutRender(REngine::Renderer& renderer) override;	// Outの場合	
	};
}
