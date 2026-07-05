//====================================================//
// ファイル名  : SceneCamera.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "SceneCamera.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
SceneCamera::SceneCamera(IComponentOwner* own)
	: CameraBase(own)
{
}

// 生成直後に一度呼ばれます
void SceneCamera::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void SceneCamera::Start()
{}

// 毎フレーム呼ばれます
void SceneCamera::Update(const GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void SceneCamera::LateUpdate(const GameTimer & gameTimer)
{}
