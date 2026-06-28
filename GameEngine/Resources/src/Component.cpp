//====================================================//
// ファイル名  : $safeitemname$.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : $shortdate$
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "$safeitemname$.h"
#include "Component.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
$safeitemname$::$safeitemname$(IComponentOwner* own)
	: WorldComponentBase(own)
{
}

// 生成直後に一度呼ばれます
void $safeitemname$::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void $safeitemname$::Start()
{}

// 毎フレーム呼ばれます
void $safeitemname$::Update(const GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void $safeitemname$::LateUpdate(const GameTimer & gameTimer)
{}
