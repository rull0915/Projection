//====================================================//
// ファイル名  : ResourceReader.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/08
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ResourceReader.h"

#include "System/ResourceManager.h"
#include "System/PrefabManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void ResourceReader::ReadModels(const std::wstring& path)
{
	// 対応している拡張子は .cmo .sdkmesh のみ
	auto paths = Read(path, { ".cmo", ".sdkmesh" });

	// パスを参照しリソースを読み込む
	for (auto& p : paths)
	{
		// ファイル名をキーに
		REngine::ResourceManager::Instance().AddModel(
			p.stem().string(),	// ファイル名	
			p.wstring()			// パス
		);
	}
}

void ResourceReader::ReadTextures(const std::wstring& path)
{
	// 対応している拡張子を列挙
	auto paths = Read(path, { ".png", ".jpeg", ".bmp", "tiff", ".gif", ".dds" });

	// パスを参照しリソースを読み込む
	for (auto& p : paths)
	{
		// ファイル名をキーに
		REngine::ResourceManager::Instance().AddTexture(
			p.stem().string(),	// ファイル名	
			p.wstring()			// パス
		);
	}
}

void ResourceReader::ReadSounds(const std::wstring& path)
{
	// 対応している拡張子は .wav のみ
	auto paths = Read(path, { ".wav" });

	// パスを参照しリソースを読み込む
	for (auto& p : paths)
	{
		// ファイル名をキーに
		REngine::ResourceManager::Instance().AddSound(
			p.stem().string(),	// ファイル名	
			p.wstring()			// パス
		);
	}
}

void ResourceReader::ReadFonts(const std::wstring& path)
{
	// 対応している拡張子は .spritefont のみ
	auto paths = Read(path, { ".spritefont" });

	// パスを参照しリソースを読み込む
	for (auto& p : paths)
	{
		// ファイル名をキーに
		REngine::ResourceManager::Instance().AddFont(
			p.stem().string(),	// ファイル名	
			p.wstring()			// パス
		);
	}
}

void ResourceReader::ReadObjects(const std::wstring& path)
{
	// 対応している拡張子は .gameobject のみ
	auto paths = Read(path, { ".gameobject" });

	// パスを参照しリソースを読み込む
	for (auto& p : paths)
	{
		// ファイル名をキーに
		REngine::PrefabManager::Instance().AddPrefab(
			p.stem().string(),	// ファイル名	
			p.wstring()			// パス
		);
	}
}

std::vector<std::filesystem::path> ResourceReader::Read(const std::wstring& path, const std::vector<std::string>& extentions)
{
	// ディレクトリを設定
	std::filesystem::path directory = path;

	// パスを受け取る配列を用意
	std::vector<std::filesystem::path> pathes(0);

	// ディレクトリのイテレータを作成しループ
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory))
	{
		// 通常のファイルであれば
		if (entry.is_regular_file())
		{
			// パスを取得
			std::filesystem::path p = entry.path();

			// 拡張子があれば
			if (std::find(extentions.begin(), extentions.end(), p.extension()) != extentions.end())
			{
				// 配列に追加
				pathes.push_back(p);
			}
		}
	}

	// 返す
	return pathes;
}
