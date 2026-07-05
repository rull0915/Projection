//====================================================//
// ファイル名  : ComponentCreator.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/30
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Loader/ComponentFactory.h"

//====================================================//
// 関数の実体宣言
//====================================================//


void ComponentFactory::Register(const std::string& name, ComponentSpace space, CreateFunc func)
{
	// イテレータを取得
	auto it = m_creatorMap.find(name);

	// 既に存在するキーなら
	if (it != m_creatorMap.end())
	{
		// 何もしない
		return;
	}

	// 新規のキーなら追加
	m_creatorMap.insert({ name, { space, func} });
}

ComponentBase* ComponentFactory::Create(const std::string & name, GameObject * owner)
{
	// イテレータを取得
	auto it = m_creatorMap.find(name);

	// 存在しないキーなら
	if (it == m_creatorMap.end())
	{
		// 何もしない
		return nullptr;
	}

	// あれば実行
	return it->second.second(owner);
}
