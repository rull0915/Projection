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

#include "Components/World/Transform/Transform.h"
#include "Components/UI/RectTransform/RectTransform.h"

struct HitContact;

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
GameObject::GameObject(CreateToken)
	: m_isActive{ true }
	, m_changedActive{ false }
	, m_parentIsActive{ true }
	, m_isDead{ false }
	, m_pScene{ nullptr }
	, m_components{ this }
	, m_tag{}
	, m_name{}
	, m_isInvincible{ false }
{
	ADD_PROPERTY(m_isActive);
	ADD_PROPERTY(m_name);
	ADD_PROPERTY(m_tag);
}

void GameObject::Finalize()
{
	// コンポーネント削除
	RemoveComponents();

	GetComponentContainer().RemoveRegistered();
}

void GameObject::OnValidate()
{
	// 変更時処理を呼び出して強制的に合わせる
	OnActiveChanged(m_parentIsActive && m_isActive);
}

void GameObject::Reserve()
{
	// 変更されていなかったら何もしない
	if (m_changedActive)
	{
		// フラグの更新
		m_isActive = !m_isActive;

		m_changedActive = false;

		// 親がtrueなら
		if (m_parentIsActive)
		{
			// 変更時処理
			OnActiveChanged(m_isActive);
		}
	}

	// コンポーネントのReserveを呼び出す
	for (auto& component : m_components.GetAll())
	{
		component->Reserve();
	}
}

GameObject* GameObject::Generate(DirectX::SimpleMath::Vector3 position)
{
	return m_pScene->GetFactory()->Generate(position);
}

void GameObject::SetParentActive(bool value)
{
	// 値が変わっていなければ何もしない
	if (m_parentIsActive == value) return;

	// フラグ更新
	m_parentIsActive = value;

	// 子がアクティブなら
	if (m_isActive)
	{
		// 変更時処理
		OnActiveChanged(m_parentIsActive);
	}
}

void GameObject::SetActive(bool f)
{
	// 現在フレーム終了時の予定状態
	bool current = m_changedActive ? !m_isActive : m_isActive;

	// 既にその状態なら何もしない
	if (current == f) return;

	// 元の状態に戻るなら変更を取り消す
	if (m_changedActive && f == m_isActive)
	{
		m_changedActive = false;
	}
	// 通常時はフラグを立てる
	else
	{
		m_changedActive = true;

		if (m_isActive)
		{
			Reserve();
		}
	}
}

void GameObject::OnActiveChanged(bool f)
{
	// 子に通知する

	// Transformの場合
	if (Transform* t = GetComponent<Transform>())
	{
		// 子を全て調べる
		for (auto& child : t->GetChildren())
		{
			// 通知
			static_cast<GameObject*>(child->GetOwn())->SetParentActive(f);
		}
	}

	// RectTransformの場合
	if (RectTransform* t = GetComponent<RectTransform>())
	{
		// 子を全て調べる
		for (auto& child : t->GetChildren())
		{
			// 通知
			static_cast<GameObject*>(child->GetOwn())->SetParentActive(f);
		}
	}

	// コンポーネントに通知する
	for (auto& component : m_components.GetAll())
	{
		component->SetOwnerActive(f);
	}
}
