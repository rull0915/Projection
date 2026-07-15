//====================================================//
// ファイル名  : ClickSE.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/15
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ClickSE.h"

#include "Components/Both/Sounds/AudioSource.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
ClickSE::ClickSE(REngine::IComponentOwner* own)
	: ButtonListenerBase(own)
{
}

void ClickSE::OnClicked()
{
	// AudioSourceがあれば
	if (auto* audio = GetComponent<REngine::AudioSource>())
	{
		audio->Play();
	}
}
