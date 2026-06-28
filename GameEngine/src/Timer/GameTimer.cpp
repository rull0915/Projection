//====================================================//
// ファイル名  : GameTimer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/04
//
// 概要       : 各オブジェクトに渡すゲーム用のタイマークラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Timer/GameTimer.h"

#include "Settings/TimeSettings.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void GameTimer::Update(float elapsedTime)
{
	m_elapsedTime = elapsedTime * TimeSettings::Instance().GetTimeScale();
	m_unscaledElapsedTime = elapsedTime;
}
