#pragma once

#define IS_COMPONENT(JumpComponent)

#include "Components/World/WorldComponentBase.h"
#include "Components/World/RigidBody/RigidBody.h"

class JumpComponent : public RHEngine::WorldComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(WorldComponentBase);

private:
	RHEngine::RigidBody* m_pRigidbody;

public:
	JumpComponent(RHEngine::IComponentOwner* own);
	~JumpComponent() = default;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;
};
