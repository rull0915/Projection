//====================================================//
// ファイル名  : PropertyTest.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/21
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "PropertyTest.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
PropertyTest::PropertyTest(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_status{}
	, m_id{ 10.0f }
{
	ADD_PROPERTY(m_status);
	ADD_PROPERTY(m_id);
	ADD_PROPERTY(m_type);
}

// 生成直後に一度呼ばれます
void PropertyTest::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void PropertyTest::Start()
{}

// 毎フレーム呼ばれます
void PropertyTest::Update(const REngine::GameTimer& gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void PropertyTest::LateUpdate(const REngine::GameTimer& gameTimer)
{}
