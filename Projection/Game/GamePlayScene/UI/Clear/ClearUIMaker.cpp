//====================================================//
// ファイル名  : ClearUIMaker.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/01
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ClearUIMaker.h"

#include "Scene/Scene.h"
#include "Managers/UI/Canvas.h"

#include "Components/UI/UIComponents.h"
#include "System/ResourceManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

ClearUIMaker::ClearUIMaker(Scene* pScene)
	: m_canvas{ nullptr }
{
	m_canvas = pScene->GenerateCanvas();
}

void ClearUIMaker::Initialize()
{
	auto obj = m_canvas->Generate();

	TextUI* text = obj->AddComponent<TextUI>();
	text->SetText("Test");
	text->SetFontSize(24);
	text->SetFont("Default");

	RectTransform* rect = text->GetComponent<RectTransform>();	
}
