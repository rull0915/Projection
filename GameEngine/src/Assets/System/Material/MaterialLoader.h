//====================================================//
// ファイル名   : MaterialLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/10
//
// 概要 : マテリアルの読み込みを行うクラス
//
// 更新履歴 :
// 2026/10/10 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <memory>
#include <utility>
#include <filesystem>
#include <string_view>

#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Types/Shader/ShaderAsset.h"
#include "Assets/Managers/AssetLoadResult.h"

namespace RHEngine
{
	class AssetContext;

	namespace Loader
	{
		//====================================================//
		// クラス宣言
		//====================================================//
		class MaterialLoader
		{


		public:

			//-----------------------------------------------------
			// コンストラクタ / デストラクタ
			//-----------------------------------------------------
			MaterialLoader() = default;
			~MaterialLoader() = default;

			//-----------------------------------------------------
			// 公開関数
			//-----------------------------------------------------

			// マテリアルを読み込む関数
			static LoadResult Load(const std::filesystem::path& path, AssetContext& ctx);
		};
	}
}
