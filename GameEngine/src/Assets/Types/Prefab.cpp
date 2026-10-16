//====================================================//
// ファイル名  : Prefab.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/28
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Types/Prefab.h"

#include "Assets/Types/PrefabImpl.h"

//====================================================//
// 関数の実体宣言
//====================================================//

RHEngine::Prefab::Prefab(std::filesystem::path path)
	: m_impl{ std::make_unique<PrefabImpl>(path) }
{
}

RHEngine::Prefab::Prefab(std::unique_ptr<PrefabImpl> impl)
	: m_impl{ std::move(impl) }
{
}
