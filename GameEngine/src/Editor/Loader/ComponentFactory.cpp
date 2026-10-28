/**
 * @file    ComponentCreator.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/06/30
 */

#include "pch.h"
#include "Editor/Loader/ComponentFactory.h"

namespace RHEngine
{
	void ComponentFactory::Register(const std::string& name, ComponentInfo space, CreateFunc func)
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

	ComponentBase* ComponentFactory::Create(const std::string& name, GameObject* pOwner)
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
		return it->second.second(pOwner);
	}
}	// namespace RHEngine
