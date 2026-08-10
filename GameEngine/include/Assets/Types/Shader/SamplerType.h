//====================================================//
// ファイル名   : SamplerType.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/08
//
// 概要 : サンプラーの種類をまとめた列挙型
//
// 更新履歴 :
// 2026/08/08 新規作成
//====================================================//

#pragma once

namespace REngine
{
	enum class SamplerType
	{
		None,
		Linear,
		Point,
		LinearClamp,
		PointClamp,
		Anisotropic,
	};
}	// namespace REngine
