//====================================================//
// ファイル名  : AssetRegistry.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/10/21
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Assets/Managers/AssetRegistry.h"

//====================================================//
// 関数の実体宣言
//====================================================//

RHEngine::AssetRegistry::AssetRegistry()
	: m_slots{}
	, m_freeIndexList{}
	, m_uuidToIndex{}
{}

RHEngine::AssetRegistry::~AssetRegistry()
{}

void RHEngine::AssetRegistry::Reset()
{
	m_slots.clear();

	m_freeIndexList.clear();

	m_uuidToIndex.clear();
}
