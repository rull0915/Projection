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

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// コンストラクタ
/// </summary>
ComponentBase::ComponentBase(IComponentOwner* own)
	: m_own{ own }
	, m_isActive{ true }
	, m_isStarted{ false }
{
	ADD_PROPATY(m_isActive);
}

/// <summary>
/// デストラクタ
/// </summary>
ComponentBase::~ComponentBase()
{
}
