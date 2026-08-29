//====================================================//
// ファイル名  : ReferenceTester.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/30
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ReferenceTester.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
ReferenceTester::ReferenceTester(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_targetTransform{}
	, m_tarrgetSetPos{}
{
	ADD_PROPERTY(m_targetTransform);
	ADD_PROPERTY(m_tarrgetSetPos);
}

// 生成直後に一度呼ばれます
void ReferenceTester::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void ReferenceTester::Start()
{}

// 毎フレーム呼ばれます
void ReferenceTester::Update(const REngine::GameTimer & gameTimer)
{
	if (m_targetTransform)
	{
		m_targetTransform->SetLocalPosition(m_tarrgetSetPos);
	}
}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void ReferenceTester::LateUpdate(const REngine::GameTimer & gameTimer)
{}
