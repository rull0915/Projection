#pragma once

#define IS_COMPONENT(MyComponent)

#include "Components/World/WorldComponentBase.h"

class MyComponent : public REngine::WorldComponentBase
{
public:
	MyComponent(REngine::IComponentOwner* own);
	~MyComponent() = default;

	void Start() override;
	void Update(const REngine::GameTimer& gameTimer) override;

	unsigned int GetID() override
	{
		return REngine::TypeIDGenerator::GetID<MyComponent>();
	}
};
