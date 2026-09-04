//====================================================//
// ファイル名  : RendererBase.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/02
//
// 概要       : レンダラーコンポーネントの基底クラス  
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/World/Renderer/RendererBase.h"

#include "Components/World/Transform/Transform.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	RendererBase::RendererBase(IComponentOwner* own)
		: WorldComponentBase(own)
		, m_pTransform{ GetComponent<Transform>() }
	{
	};
}	// namespace REngine
