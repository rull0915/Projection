//====================================================//
// ファイル名  : ComponentBase.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/02
//
// 概要       : コンポーネントの基底クラス  
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/ComponentBase.h"

#include "GameObject/Interface/IComponentOwner.h"
#include "Components/World/Transform/Transform.h"

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
ComponentBase::ComponentBase(IComponentOwner* own)
	: m_own{ own }
	, m_changedActive{ false }
	, m_isActive{ true }
	, m_ownerIsActive{ true }
	, m_isStarted{ false }
{
	ADD_PROPERTY(m_isActive);
}

/// <summary>
/// デストラクタ
/// </summary>
ComponentBase::~ComponentBase()
{
}

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
