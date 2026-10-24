//====================================================//
// ファイル名  : RangeTester.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/23
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "RangeTester.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
RangeTester::RangeTester(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_range{ 0.0f, 1.0f }
{
	ADD_PROPERTY(RangeTester, m_range);
}

// 最初のUpdate関数の直線に一度呼ばれます
void RangeTester::Start()
{}

// 毎フレーム呼ばれます
void RangeTester::Update(const RHEngine::GameTimer & gameTimer)
{}
