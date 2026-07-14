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
	// 変化がないなら何もしない
	if (m_isActive == f) return;

	// フラグを更新
	m_isActive = f;

	// 所有者がtrueなら
	if (m_ownerIsActive)
	{
		// 変更時処理
		OnActiveChanged(m_isActive);
	}
}

void ComponentBase::OnActiveChanged(bool f)
{
	// スタートが呼ばれる前は通知しない
	if (!m_isStarted) return;
	
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
