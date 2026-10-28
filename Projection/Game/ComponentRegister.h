/**
 * @file    ComponentRegister.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/11
 *
 * @brief   コンポーネントの登録をするクラス
 */
#pragma once

#include "Editor/Loader/ComponentFactory.h"
#include "GameObject/GameObject.h"

// 生成関数登録マクロ
#define REGIST_TO_FACTORY(Name) (RHEngine::ComponentFactory::Register(#Name, { RHEngine::ComponentProject::Game, Name::SPACE }, [](RHEngine::GameObject* o){ return o->AddComponent<Name>(); }))

class ComponentRegister
{
public:

	ComponentRegister() = default;
	~ComponentRegister() = default;

	static void RegistComponents();
};
