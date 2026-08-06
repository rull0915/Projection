//====================================================//
// ファイル名   : ShaderLoader.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/04
//
// 概要 : シェーダーアセットのロードに関連する関数群
//
// 更新履歴 :
// 2026/08/04 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <memory>
#include <filesystem>

namespace REngine
{
	class ShaderAsset;

	// ロード関数群名前空間
	namespace Loader
	{
		// ロード関数
		std::unique_ptr<ShaderAsset> ShaderLoader(const std::filesystem::path& path);
	}
}
