//====================================================//
// ファイル名  : MyComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/22
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "MyComponent.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
MyComponent::MyComponent(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
{
}

// 最初のUpdate関数の直線に一度呼ばれます
void MyComponent::Start()
{}

// 毎フレーム呼ばれます
void MyComponent::Update(const REngine::GameTimer & gameTimer)
{}
