/**
 * @file    ComponentBase.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/02
 */

#include "pch.h"
#include "Components/ComponentBase.h"

#include "GameObject/Interface/IComponentOwner.h"

namespace RHEngine 
{
	ComponentBase::ComponentBase(IComponentOwner* pOwner)
		: m_pOwner{ pOwner }
		, m_uuid{ 0 }
		, m_changedActive{ false }
		, m_isActive{ true }
		, m_ownerIsActive{ true }
		, m_isStarted{ false }
	{
		ADD_PROPERTY(ComponentBase, m_isActive);
	}

	ComponentBase::~ComponentBase()
	{}

	void ComponentBase::SetOwnerActive(bool f)
	{
		// 変化がないなら何もしない
		if (m_ownerIsActive == f) return;

		// フラグを更新
		m_ownerIsActive = f;

		// 自分がtrueなら
		if (m_isActive)
		{
			// 変更時処理
			OnActiveChanged(m_ownerIsActive);
		}
	}

	void ComponentBase::SetActive(bool f)
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

	void ComponentBase::Reserve()
	{
		// 変更されていなかったら何もしない
		if (!m_changedActive) return;

		// フラグの更新
		m_isActive = !m_isActive;

		m_changedActive = false;

		// 所有者がtrueなら
		if (m_ownerIsActive)
		{
			// 変更時処理
			OnActiveChanged(m_isActive);
		}
	}

	void ComponentBase::OnActiveChanged(bool f)
	{
		// アクティブへ変更時
		if (f)
		{
			OnEnable();
		}
		// 非アクティブへ変更時
		else
		{
			OnDisable();
		}
	}
} // namespace RHEngine
