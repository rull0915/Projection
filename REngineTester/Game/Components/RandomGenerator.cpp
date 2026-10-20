//====================================================//
// ファイル名  : RandomGenerator.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/19
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "RandomGenerator.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
RandomGenerator::RandomGenerator(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_prefab{}
	, m_min{}
	, m_max{}
	, m_generateType{}
	, m_generateCount{}
{
	ADD_PROPERTY(RandomGenerator, m_prefab);
	ADD_PROPERTY(RandomGenerator, m_min);
	ADD_PROPERTY(RandomGenerator, m_max);
	ADD_PROPERTY(RandomGenerator, m_generateType);
	ADD_PROPERTY(RandomGenerator, m_generateCount);
}

// 最初のUpdate関数の直線に一度呼ばれます
void RandomGenerator::Start()
{
	// 生成
	for (int i = 0; i < m_generateCount; i++)
	{
		// シーンの取得
		RHEngine::GameObject* pGameObject = static_cast<RHEngine::GameObject*>(GetOwn());
		RHEngine::Scene* pScene = pGameObject->GetScene();

		// 生成
		RHEngine::GameObject*obj = pScene->GetFactory()->Instantiate(m_prefab);

		// ランダム
		obj->GetComponent<RHEngine::Transform>();
	}
}

// 毎フレーム呼ばれます
void RandomGenerator::Update(const RHEngine::GameTimer & gameTimer)
{}
