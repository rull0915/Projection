/**
 * @file    OpenFileDialog.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/06
 *
 * @brief   ファイル選択のダイアログを管理する
 */

#pragma once

#include <string>

namespace RHEngine
{
	/**
	 * @brief 開くモード
	 */
	namespace FileDialog
	{
		enum class Mode
		{
			Save,
			Open,
		};

		/**
		 * @brief ファイル選択ダイアログを開く
		 * @param mode モード
		 * @param initPath 初期状態で開くディレクトリのパス
		 * @param extention 取得する拡張子
		 * @return 開いたファイルへのパス
		 */
		std::wstring Open(Mode mode, const std::wstring& initPath, const std::wstring& extention);
	}
}	// namespace RHEngine
