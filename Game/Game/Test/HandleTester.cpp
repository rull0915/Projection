//====================================================//
// ファイル名  : HandleTester.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/27
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "HandleTester.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
HandleTester::HandleTester(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
{
	ADD_PROPERTY(m_textureHandle);
}

// 生成直後に一度呼ばれます
void HandleTester::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void HandleTester::Start()
{}

// 毎フレーム呼ばれます
void HandleTester::Update(const REngine::GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void HandleTester::LateUpdate(const REngine::GameTimer & gameTimer)
{}
