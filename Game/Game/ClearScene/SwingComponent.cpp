//====================================================//
// ファイル名  : SwingComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/14
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "SwingComponent.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
SwingComponent::SwingComponent(REngine::IComponentOwner* own)
	: UIComponentBase(own)
	, m_rectTransform{ nullptr }
{
}

// 生成直後に一度呼ばれます
void SwingComponent::Awake()
{
	// RectTransformを取得
	m_rectTransform = GetComponent<REngine::RectTransform>();
}

// 毎フレーム呼ばれます
void SwingComponent::Update(const REngine::GameTimer & gameTimer)
{
	static float e = 0;
	e += gameTimer.GetElapsedTime();

	// 揺らす
	m_rectTransform->SetRotation(std::sinf(e * 2) * 0.5f);
}
