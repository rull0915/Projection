//====================================================//
// ファイル名  : RegisterTest.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/11
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "RegisterTest.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
RegisterTest::RegisterTest(IComponentOwner* own)
	: WorldComponentBase(own)
{
}

// 生成直後に一度呼ばれます
void RegisterTest::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void RegisterTest::Start()
{}

// 毎フレーム呼ばれます
void RegisterTest::Update(const GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void RegisterTest::LateUpdate(const GameTimer & gameTimer)
{}
