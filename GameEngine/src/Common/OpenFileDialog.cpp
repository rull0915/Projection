//====================================================//
// ファイル名  : OpenFileDialog.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/06
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//

#include "pch.h"
#include <Windows.h>
#include <shobjidl.h>
#include <filesystem>

#include "OpenFileDialog.h"

namespace REngine
{
	std::wstring FileDialog::Open(Mode mode, const std::wstring& initPath, const std::wstring& extention)
	{
		// COMの初期化
		bool coInit = SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

		// ダイアログオブジェクトのポインタを作成
		IFileDialog* pFile = nullptr;

		// 選択したファイルのパスを受け取る変数の作成
		std::wstring resultPath = L"";

		// 現在の作業ディレクトリを取得
		std::filesystem::path current = std::filesystem::current_path();

		// 相対パスを付け足す
		current /= initPath;

		// ファイルの区切りを\\に統一する。
		current.make_preferred();

		// インスタンス作成
		HRESULT hr;
		switch (mode)
		{
			// Saveの場合
		case FileDialog::Mode::Save:
			hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFile));
			break;

			// Openの場合
		case FileDialog::Mode::Open:
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFile));
			break;

		default:
			return std::wstring();
			break;
		}

		// 成功していたら
		if (SUCCEEDED(hr))
		{
			// 初期ディレクトリを指定する変数を用意
			Microsoft::WRL::ComPtr<IShellItem> folder;

			// 作成
			hr = SHCreateItemFromParsingName(
				current.wstring().c_str(),
				nullptr,
				IID_PPV_ARGS(&folder));

			// 反映する
			if (SUCCEEDED(hr))
			{
				pFile->SetFolder(folder.Get());
			}

			// フィルターの設定
			std::wstring filter = L"*" + extention;

			// フィルター構造体の配列
			COMDLG_FILTERSPEC fileTypes[] = {
				// フィルター名と拡張子をペアで保存
				{ L"File", filter.c_str() }
			};

			// 作成したフィルターを反映	: 要素数と実体
			pFile->SetFileTypes(_countof(fileTypes), fileTypes);

			// ユーザーが拡張子を省略して入力した際に自動で付けられる拡張子の指定
			pFile->SetDefaultExtension(extention.c_str());

			// ダイアログを表示
			hr = pFile->Show(NULL);

			// 表示が無事終了したら　(キャンセルや×で閉じられた場合falseになる)
			if (SUCCEEDED(hr))
			{
				// 選択結果を受け取る変数の作成
				IShellItem* pItem = nullptr;

				// 結果の取得
				hr = pFile->GetResult(&pItem);

				// 成功したら
				if (SUCCEEDED(hr))
				{
					// 名前を受け取る変数の作成
					PWSTR pszFilePath = nullptr;

					// 名前を受け取る 
					// 参考 : https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/ne-shobjidl_core-sigdn
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// 成功したら
					if (SUCCEEDED(hr))
					{
						// 用意した変数に代入
						resultPath = pszFilePath;
						CoTaskMemFree(pszFilePath); // メモリ解放
					}

					// 解放
					pItem->Release();
				}
			}

			// 解放
			pFile->Release();
		}

		// COMの使用を終了
		if (coInit) CoUninitialize();

		// パスを返す
		return resultPath;
	}
}	// namespace REngine
