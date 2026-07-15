//====================================================//
// ファイル名   : OpenFileDialog.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要 : ファイルを開く関数群
//
// 更新履歴 :
// 2026/07/06 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <string>

namespace REngine
{
	//====================================================//
	// 関数宣言
	//====================================================//

	namespace FileDialog
	{
		enum class Mode
		{
			Save,
			Open,
		};

		std::wstring Open(Mode mode, const std::wstring& initPath, const std::wstring& extention);

		std::wstring LoadFileDialog(const std::wstring& initPath, const std::wstring& extention);
	}
}	// namespace REngine
