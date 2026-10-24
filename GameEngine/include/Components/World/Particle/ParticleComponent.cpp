//====================================================//
// ファイル名  : ParticleComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/23
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ParticleComponent.h"

namespace  RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	ParticleComponent::ParticleComponent(IComponentOwner* own)
		: WorldComponentBase(own)
	{}

	// 最初のUpdate関数の直線に一度呼ばれます
	void ParticleComponent::Start()
	{}

	// 毎フレーム呼ばれます
	void ParticleComponent::Update(const GameTimer& gameTimer)
	{}
}
