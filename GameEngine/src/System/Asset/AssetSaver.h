//====================================================//
// ファイル名   : AssetSaver.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/02
//
// 概要 : アセットの保存関数をまとめたヘッダ
//
// 更新履歴 :
// 2026/08/02 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//


#include "Assets/Types/Texture.h"
#include "Assets/Types/Prefab.h"

#include "Editor/Saver/ObjectSaver.h"

namespace REngine
{
	namespace Saver
	{		
		// 各ロード関数

		// Prefab
		void PrefabSaver(AssetBase* prefab, const std::wstring& path);

		// Propertyとして扱う場合
		void AssetSaverAsProperty(AssetBase* asset, const std::wstring& path, AssetManager& assetManager);
	}
}	// namespace REngine
