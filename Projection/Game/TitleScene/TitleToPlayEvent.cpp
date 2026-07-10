//====================================================//
// ファイル名  : TitleToPlayEvent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/11
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "TitleToPlayEvent.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

#include "GameLib/Transition/SlideTransition.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
TitleToPlayEvent::TitleToPlayEvent(IComponentOwner* own)
	: ButtonListenerBase(own)
{
}

void TitleToPlayEvent::OnClicked()
{
	// シーンポインタを取得
	Scene* pScene = static_cast<GameObject*>(GetOwn())->GetScene();

	pScene->ChangeScene("GamePlay",
		std::make_unique<Transition::Slide>(0.3f, DirectX::SimpleMath::Color{ 0, 0, 0, 1 }, DirectX::XMConvertToRadians(30)),
		std::make_unique<Transition::Slide>(0.3f, DirectX::SimpleMath::Color{ 0, 0, 0, 1 }, DirectX::XMConvertToRadians(210))
	);
}
