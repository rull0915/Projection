//====================================================//
// ファイル名  : MoveComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/02
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "MoveComponent.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
MoveComponent::MoveComponent(IComponentOwner* own)
	: WorldComponentBase(own)
{
}

// 生成直後に一度呼ばれます
void MoveComponent::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void MoveComponent::Start()
{}

// 毎フレーム呼ばれます
void MoveComponent::Update(const GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void MoveComponent::LateUpdate(const GameTimer & gameTimer)
{}
