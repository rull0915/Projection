//====================================================//
// ファイル名   : TransitionManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要 : シーン遷移管理クラス
//
// 更新履歴 :
// 2026/06/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "TransitionBase.h"

#include <memory>
#include <functional>

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class Renderer;

	//====================================================//
	// クラス宣言
	//====================================================//
	class TransitionManager
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 今のモード
		Transition::Mode m_nowMode;

		// Out演出
		std::unique_ptr<Transition::Base> m_outTransition;

		// In演出
		std::unique_ptr<Transition::Base> m_inTransition;

		// Out終了時,Inの直前に呼ばれる関数
		std::function<void()> m_func;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		TransitionManager()
			: m_outTransition{}
			, m_inTransition{}
			, m_func{}
			, m_nowMode{ Transition::Mode::None }
		{}

		~TransitionManager() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 更新関数
		void Update(const GameTimer& gameTimer);

		// 描画関数
		void Render(Renderer& renderer);

		// 遷移演出を開始する関数
		void StartTrans(
			std::unique_ptr<Transition::Base> outTrans,
			std::unique_ptr<Transition::Base> inTrans,
			std::function<void()> func
		);

		// 演出中かどうかを返す関数
		bool IsTransitioning() const;
	};
}	// namespace REngine
