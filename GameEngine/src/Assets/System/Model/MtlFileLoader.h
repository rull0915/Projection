//====================================================//
// ファイル名   : MtlFileLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/13
//
// 概要 : .mtlファイルからマテリアルを生成するクラス
//
// 更新履歴 :
// 2026/10/13 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <filesystem>
#include <unordered_map>

#include "Assets/Types/Material/MaterialAsset.h"
#include "Assets/Managers/AssetContext.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class MtlFileLoader
	{
	public:

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		static std::unordered_map<std::string, MaterialAsset> LoadMTL(const std::filesystem::path& path, AssetContext& ctx);
	};
}
