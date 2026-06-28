//====================================================//
// ファイル名  : TransitionManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要       : シーン遷移管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Scene/Transition/TransitionManager.h"
#include "Renderer/Renderer.h"

#include <utility>

//====================================================//
// 関数の実体宣言
//====================================================//

void TransitionManager::Update(const GameTimer& gameTimer)
{
	// Out演出中の場合
	if (m_nowMode == Transition::Mode::Out)
	{
		// out演出が設定されていたら
		if (m_outTransition)
		{
			// 演出が終わったら
			if (m_outTransition->OutUpdate(gameTimer))
			{
				// Inへ
				m_nowMode = Transition::Mode::In;

				// 関数呼び出し
				m_func();

				// Outを解放
				m_outTransition = nullptr;

				// In演出の初期化
				if (m_inTransition) m_inTransition->Initialize();
			}
		}
		// 設定されていなければ
		else
		{
			// 即Inへ移行

			// Inへ
			m_nowMode = Transition::Mode::In;

			// 関数呼び出し
			m_func();

			// Outを解放
			m_outTransition = nullptr;

			// In演出の初期化
			if (m_inTransition) m_inTransition->Initialize();
		}
	}
	// In演出中の場合
	else if (m_nowMode == Transition::Mode::In)
	{
		// In演出が設定されていれば
		if (m_inTransition)
		{
			// 演出が終わったら
			if (m_inTransition->InUpdate(gameTimer))
			{
				// Noneへ
				m_nowMode = Transition::Mode::None;

				// Inを解放
				m_inTransition = nullptr;
			}
		}
		// されていなければ
		else
		{
			// 即演出終了
			
			// Noneへ
			m_nowMode = Transition::Mode::None;

			// Inを解放
			m_inTransition = nullptr;
		}
	}
}

void TransitionManager::Render(Renderer& renderer)
{
	// Outなら
	if (m_nowMode == Transition::Mode::Out && m_outTransition)
	{
		// Out演出の描画
		m_outTransition->OutRender(renderer);
	}
	// Inなら
	else if (m_nowMode == Transition::Mode::In && m_inTransition)
	{
		// In演出の描画
		m_inTransition->InRender(renderer);
	}
}

void TransitionManager::StartTrans(std::unique_ptr<Transition::Base> outTrans, std::unique_ptr<Transition::Base> inTrans, std::function<void()> func)
{
	// 設定
	m_outTransition = std::move(outTrans);
	m_inTransition = std::move(inTrans);

	m_func = func;

	// 開始
	m_nowMode = Transition::Mode::Out;

	// Out演出の初期化
	if(m_outTransition) m_outTransition->Initialize();
}

bool TransitionManager::IsTransitioning() const
{
	// モードがNoneなら演出中ではない
	return m_nowMode != Transition::Mode::None;
}
