//====================================================//
// ファイル名  : ToSelectComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/12
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ToSelectComponent.h"

#include "Scene/SceneManager.h"
#include "Scene/Transition/FadeTransition.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
ToSelectComponent::ToSelectComponent(IComponentOwner* own)
	: ButtonListenerBase(own)
{
}

void ToSelectComponent::OnClicked()
{
	SceneManager::Instance().RequestSceneChange("Select",
		std::make_unique<Transition::Fade>(0.3f),
		std::make_unique<Transition::Fade>(0.3f)
	);
}
