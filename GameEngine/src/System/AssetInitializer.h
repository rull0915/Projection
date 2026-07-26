//====================================================//
// ファイル名   : AssetInitializer.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : エンジンにデフォルトで存在するアセットの初期化を行うクラス
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <string>
#include <memory>

#include "Assets/Managers/AssetManager.h"
#include "Assets/Types/Texture.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class AssetInitializer
	{
	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		AssetInitializer() = default;
		~AssetInitializer() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 初期化
		static void AssetInitialize(AssetManager& assetManager);
	};

	namespace Loader
	{		
		// 各ロード関数

		// Texture
		std::unique_ptr<Texture> TextureLoader(const std::wstring& path);
	}

}	// namespace REngine
