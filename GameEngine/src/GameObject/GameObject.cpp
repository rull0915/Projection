/**
 * @file    GameObject.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/01
 */

#include "pch.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

#include "Components/World/Transform/Transform.h"
#include "Components/UI/RectTransform/RectTransform.h"
#include "Scene/ObjectFactory.h"

namespace RHEngine 
{
	GameObject::GameObject(CreateToken)
		: m_isActive{ true }
		, m_changedActive{ false }
		, m_parentIsActive{ true }
		, m_isDead{ false }
		, m_pScene{ nullptr }
		, m_components{ this }
		, m_tag{}
		, m_name{ "GameObject" }
		, m_isSerializable{ true }
		, m_uuid{ 0 }
	{
		ADD_PROPERTY(GameObject, m_isActive);
		ADD_PROPERTY(GameObject, m_name);
		ADD_PROPERTY(GameObject, m_tag);
	}

	void GameObject::Finalize()
	{
		// シーンに通知
		if (m_pScene) m_pScene->OnGameObjectDestroy(this);

		// コンポーネントを全削除
		m_components.Removes<ComponentBase>();

		GetComponentContainer().RemoveRegistered();
	}

	void GameObject::OnValidate()
	{
		// 変更時処理を呼び出して強制的に合わせる
		OnActiveChanged(m_parentIsActive && m_isActive);
	}

	void GameObject::ActiveReserve()
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
		for (auto& pComponent : GetAllComponents())
		{
			pComponent->Reserve();
		}
	}

	GameObject* GameObject::Generate(DirectX::SimpleMath::Vector3 position)
	{
		if (m_pScene)
		{
			return m_pScene->GetFactory()->Generate(position);
		}
		return nullptr;
	}

	GameObject* GameObject::GetParent() const
	{
		if (auto* pTransform = GetComponent<Transform>())
		{
			// 親がいれば
			if (auto* pParent = pTransform->GetParent())
			{
				return static_cast<GameObject*>(pParent->GetOwn());
			}
		}
		else if (auto* pRectTransform = GetComponent<RectTransform>())
		{
			// 親がいれば
			if (auto* pParent = pRectTransform->GetParent())
			{
				return static_cast<GameObject*>(pParent->GetOwn());
			}
		}

		return nullptr;
	}

	size_t GameObject::GetChildCount() const
	{
		if (auto* pTransform = GetComponent<Transform>()) return pTransform->GetChildren().size();
		else if (auto* pRectTransform = GetComponent<RectTransform>()) return pRectTransform->GetChildren().size();

		return 0;	
	}

	std::vector<GameObject*> GameObject::GetChildren() const
	{
		// 子オブジェクトを保持する配列を用意
		std::vector<GameObject*> children{};

		// Transformを持っているとき
		if (auto* pTransform = GetComponent<Transform>())
		{
			// 子を走査
			for (auto& pChild : pTransform->GetChildren())
			{
				// 配列に追加
				children.push_back(static_cast<GameObject*>(pChild->GetOwn()));
			}
		}

		// RectTransformを持っているとき
		else if (auto* pRectTransform = GetComponent<RectTransform>())
		{
			// 子を走査
			for (auto& pChild : pRectTransform->GetChildren())
			{
				// 配列に追加
				children.push_back(static_cast<GameObject*>(pChild->GetOwn()));
			}
		}

		return children;
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
				ActiveReserve();
			}
		}
	}

	void GameObject::Destroy()
	{
		// 自身の死亡フラグをオンに
		m_isDead = true;

		// 子オブジェクトも消す
		for (auto pChild : GetChildren())
		{
			pChild->Destroy();
		}
	}

	void GameObject::OnCollisionEnter(HitContact& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnCollisionEnter(contact);
	}
	void GameObject::OnCollisionStay(HitContact& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnCollisionStay(contact);
	}
	void GameObject::OnCollisionExit(HitContact& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnCollisionExit(contact);
	}
	void GameObject::OnTriggerEnter(HitContact& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnTriggerEnter(contact);
	}
	void GameObject::OnTriggerStay(HitContact& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnTriggerStay(contact);
	}
	void GameObject::OnTriggerExit(HitContact& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnTriggerExit(contact);
	}

	void GameObject::OnCollisionEnter2D(HitContact2D& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnCollisionEnter2D(contact);
	}
	void GameObject::OnCollisionStay2D(HitContact2D& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnCollisionStay2D(contact);
	}
	void GameObject::OnCollisionExit2D(HitContact2D& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnCollisionExit2D(contact);
	}
	void GameObject::OnTriggerEnter2D(HitContact2D& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnTriggerEnter2D(contact);
	}
	void GameObject::OnTriggerStay2D(HitContact2D& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnTriggerStay2D(contact);
	}
	void GameObject::OnTriggerExit2D(HitContact2D& contact)
	{
		for (auto& pComponent : GetAllComponents()) pComponent->OnTriggerExit2D(contact);
	}

	void GameObject::OnActiveChanged(bool f)
	{
		// 子に通知する

		// Transformの場合
		if (Transform* pTransform = GetComponent<Transform>())
		{
			// 子を全て調べる
			for (auto& pChild : pTransform->GetChildren())
			{
				// 通知
				static_cast<GameObject*>(pChild->GetOwn())->SetParentActive(f);
			}
		}

		// RectTransformの場合
		if (RectTransform* pRectTransform = GetComponent<RectTransform>())
		{
			// 子を全て調べる
			for (auto& pChild : pRectTransform->GetChildren())
			{
				// 通知
				static_cast<GameObject*>(pChild->GetOwn())->SetParentActive(f);
			}
		}

		// コンポーネントに通知する
		for (auto& pComponent : GetAllComponents())
		{
			pComponent->SetOwnerActive(f);
		}
	}
} // namespace RHEngine
