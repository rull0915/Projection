/**
 * @file    RandomGenerator.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/19
 */

#include "pch.h"
#include "RandomGenerator.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/ObjectFactory.h"

#include "Common/Random.h"

RandomGenerator::RandomGenerator(RHEngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_prefab{}
	, m_minPos{}
	, m_maxPos{}
	, m_minScale{}
	, m_maxScale{}
	, m_generateType{}
	, m_generateCount{}
{
	ADD_PROPERTY(RandomGenerator, m_prefab);
	ADD_PROPERTY(RandomGenerator, m_minPos);
	ADD_PROPERTY(RandomGenerator, m_maxPos);
	ADD_PROPERTY(RandomGenerator, m_minScale);
	ADD_PROPERTY(RandomGenerator, m_maxScale);
	ADD_PROPERTY(RandomGenerator, m_generateType);
	ADD_PROPERTY(RandomGenerator, m_generateCount);
}

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
		RHEngine::Transform* t = obj->GetComponent<RHEngine::Transform>();

		// World空間のオブジェクトでなければ削除
		if (!t)
		{
			obj->Destroy();

			return;
		}

		// 指定された要素をランダムで設定

		// 位置
		if (m_generateType & RandomType::Position)
		{
			DirectX::SimpleMath::Vector3 pos = {
				RHEngine::Random::Get(m_minPos.x, m_maxPos.x),
				RHEngine::Random::Get(m_minPos.y, m_maxPos.y),
				RHEngine::Random::Get(m_minPos.z, m_maxPos.z)
			};

			t->SetLocalPosition(pos);
		}

		// 角度
		if (m_generateType & RandomType::Rotation)
		{
			DirectX::SimpleMath::Vector3 rot = {
				RHEngine::Random::Get(0.0f, DirectX::XM_2PI),
				RHEngine::Random::Get(0.0f, DirectX::XM_2PI),
				RHEngine::Random::Get(0.0f, DirectX::XM_2PI)
			};

			t->SetLocalEulerAngle(rot);
		}

		// スケール
		if (m_generateType & RandomType::Scale)
		{
			DirectX::SimpleMath::Vector3 rot = {
				RHEngine::Random::Get(m_minScale.x, m_maxScale.x),
				RHEngine::Random::Get(m_minScale.y, m_maxScale.y),
				RHEngine::Random::Get(m_minScale.z, m_maxScale.z)
			};

			t->SetLocalScale(rot);
		}
	}
}

void RandomGenerator::Update(const RHEngine::GameTimer & gameTimer)
{}
