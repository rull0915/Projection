//====================================================//
// ファイル名  : ArrayTester.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/09/03
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ArrayTester.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
ArrayTester::ArrayTester(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_testArray{ 1, 2, 4 }
	, m_colorArray(1)
{
	ADD_PROPERTY(ArrayTester, m_testArray);
	ADD_PROPERTY(ArrayTester, m_colorArray);
}

// 最初のUpdate関数の直線に一度呼ばれます
void ArrayTester::Start()
{}

// 毎フレーム呼ばれます
void ArrayTester::Update(const REngine::GameTimer&)
{}
