#include "pch.h"
#include "JumpComponent.h"

#include "Input/KeyInput.h"

using namespace RHEngine;
using namespace DirectX;

JumpComponent::JumpComponent(IComponentOwner* own)
	: WorldComponentBase(own)
	, m_pRigidbody{ nullptr }
{
}

void JumpComponent::Start()
{
	m_pRigidbody = GetComponent<RigidBody>();
}

void JumpComponent::Update(const GameTimer & gameTimer)
{
	const float JUMP_POWER = 10.0f;

	if (Input::Key::GetDown(Input::Key::Code::Space))
	{
		m_pRigidbody->AddForce(
			JUMP_POWER * SimpleMath::Vector3::Up, 
			ForceMode::Impulse
		);
	}
}
