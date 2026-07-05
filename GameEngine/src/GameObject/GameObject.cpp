//====================================================//
// ファイル名  : GameObject.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/01
//
// 概要        : ゲームオブジェクト基底クラスです
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

struct HitContact;

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
GameObject::GameObject(CreateToken)
	: m_isActive{ true }
	, m_isDead{ false }
	, m_pScene{ nullptr }
	, m_components{ this }
	, m_tag{}
	, m_name{}
	, m_isInvincible{ false }
{
	ADD_PROPERTY(m_name);
	ADD_PROPERTY(m_tag);
}

void GameObject::BaseUpdate(const GameTimer& gameTimer)
{
	m_components.UpdateComponets(gameTimer);
}

void GameObject::BaseLateUpdate(const GameTimer& gameTimer)
{
	m_components.LateUpdateComponents(gameTimer);
}

void GameObject::BaseFinalize()
{
	// コンポーネント削除
	RemoveComponents();

	GetComponentContainer().RemoveRegistered();
}

GameObject* GameObject::Generate(DirectX::SimpleMath::Vector3 position)
{
	return m_pScene->Generate(position);
}
