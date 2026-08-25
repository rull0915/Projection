//====================================================//
// ファイル名  : Dialog.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/25
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Dialog.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
Dialog::Dialog(REngine::IComponentOwner* own)
	: UIComponentBase(own)
{
}

// 生成直後に一度呼ばれます
void Dialog::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void Dialog::Start()
{}

// 毎フレーム呼ばれます
void Dialog::Update(const REngine::GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void Dialog::LateUpdate(const REngine::GameTimer & gameTimer)
{}
