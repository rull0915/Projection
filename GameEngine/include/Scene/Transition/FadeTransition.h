//====================================================//
// ファイル名   : FadeTransition.h
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
#include "TransitionBase.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//

namespace Transition
{
	class Fade : public Base
	{
	private:
		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		float m_elapsedTime;

		float m_transSec;

		DirectX::SimpleMath::Color m_fadeColor;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Fade(float sec = 0.5f, DirectX::SimpleMath::Color color = { 0, 0, 0, 1 })
			: m_transSec{ sec }
			, m_fadeColor{ color }
			, m_elapsedTime{ 0.0f }
		{
		}
		~Fade() = default;

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
