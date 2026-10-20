#pragma once

#define IS_COMPONENT(JumpComponent)

#include "Components/World/WorldComponentBase.h"
#include "Components/World/RigidBody/RigidBody.h"

class JumpComponent : public RHEngine::WorldComponentBase
{
private:
	RHEngine::RigidBody* m_pRigidbody;

public:
	JumpComponent(RHEngine::IComponentOwner* own);
	~JumpComponent() = default;

	COMPONENT_TYPE(JumpComponent, RHEngine::WorldComponentBase);
	
	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};
