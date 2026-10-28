/**
 * @file    RandomGenerator.h
 * @author  Hoshino Ryunosuke
 * @date    2026/10/19
 *
 * @brief   ランダムにPrefabを生成するコンポーネント
 */

#pragma once

#include "Components/World/WorldComponentBase.h"
#include "Assets/Types/Prefab.h"

class RandomGenerator : public RHEngine::WorldComponentBase
{
public:
	enum RandomType
	{
		Position	= 0b001,
		Rotation	= 0b010,
		Scale		= 0b100,
		PosRot		= Position | Rotation,
		PosSca		= Position | Scale,
		RotSca		= Rotation | Scale,
		All			= Position | Rotation | Scale
	};

private:

	// 生成対象のPrefab
	RHEngine::Handle<RHEngine::Prefab> m_prefab;

	// 生成範囲
	DirectX::SimpleMath::Vector3 m_minPos;
	DirectX::SimpleMath::Vector3 m_maxPos;

	DirectX::SimpleMath::Vector3 m_minScale;
	DirectX::SimpleMath::Vector3 m_maxScale;

	// 生成方法
	RandomType m_generateType;

	// 生成個数
	int m_generateCount;

public:

	RandomGenerator(RHEngine::IComponentOwner* own);
	~RandomGenerator() = default;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};
