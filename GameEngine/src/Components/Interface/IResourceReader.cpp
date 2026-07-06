//====================================================//
// ファイル名  : IResourceReader.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/Interface/IResourceReader.h".h"

#include "System/ResourceManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

bool IResourceReader::LoadResource()
{
	void** target = GetMyResource();

	switch (GetType())
	{
		// Modelの場合
	case IResourceReader::Type::Model:
		*target = ResourceManager::Instance().GetModel(GetKeyName());
		break;

		// Textureの場合
	case IResourceReader::Type::Texture:
		*target = ResourceManager::Instance().GetTexture(GetKeyName());
		break;

		// Soundの場合
	case IResourceReader::Type::Sound:
		*target = ResourceManager::Instance().GetSound(GetKeyName());
		break;

		// Fontの場合
	case IResourceReader::Type::Font:
		*target = ResourceManager::Instance().GetSpriteFont(GetKeyName());
		break;

		// Objectの場合
	case IResourceReader::Type::Object:
		break;

	default:
		break;
	}

    return target;
}
