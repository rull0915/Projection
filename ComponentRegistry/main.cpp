/**
 * @brief	コンポーネントの登録関数を自動生成するプロジェクト
 * @author	Hoshino Ryunosuke
 * @date	2026/10/29
 */

#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <Windows.h>

std::filesystem::path GetCurrentModulePath();

bool HasProjectFile(const std::filesystem::path& directory);

std::vector<std::string> CheckHeaderFile(const std::filesystem::path& targetFile);

int main()
{
	//// .exeへのパスを取得
	//auto currentPath = GetCurrentModulePath();

	//// 親ファイルを操作対象にする
	//auto target = currentPath.parent_path();

	//// プロジェクトファイルがなければ調査しない
	//if (!HasProjectFile(target)) return -1;

	//// ファイルを再帰的に調べる
	//for (auto& file : std::filesystem::recursive_directory_iterator(target))
	//{
	//	// ヘッダファイルなら中身を調べる
	//	if (file.path().extension() == ".h")
	//	{

	//	}
	//}

	auto a = CheckHeaderFile("MyHeader.h");

	return 0;
}

/**
 * @brief カレントディレクトリを取得する
 */
std::filesystem::path GetCurrentModulePath()
{
	// 初期サイズとしてMAX_PATHを指定
	DWORD capacity = MAX_PATH; 

	std::wstring path;

	while (true)
	{
		path.resize(capacity);

		// ディレクトリを取得する
		DWORD result = GetModuleFileNameW(NULL, path.data(), capacity);

		// 文字数が指定した容量より多かった場合切り詰められている
		if (result >= capacity) 
		{
			capacity *= 2; // バッファサイズを2倍にして再度調べる
			continue;
		}

		// 成功したら正しい長さに
		path.resize(result);

		break;
	}

	return static_cast<std::filesystem::path>(path);
}

/**
 * @brief ディレクトリ直下にプロジェクトファイルがあるかどうか調べる
 * @param directory 調査対象のディレクトリ
 * @return 
 */
bool HasProjectFile(const std::filesystem::path& directory)
{
	// ディレクトリじゃない場合は調べない
	if (!std::filesystem::is_directory(directory)) return false;

	// 直下のファイルのみを調べる
	for (auto& file : std::filesystem::directory_iterator(directory))
	{
		// プロジェクトファイルなら
		if (file.path().extension() == ".vcxproj")
		{
			return true;
		}
	}

	return false;
}

/**
 * @brief 単一のヘッダファイルを調べる
 * @param targetFile 対象のファイルパス
 * @return 見つかったコンポーネントクラス名の一覧
 */
std::vector<std::string> CheckHeaderFile(const std::filesystem::path& targetFile)
{
	// 配列を用意
	std::vector<std::string> fileList{};

	// ファイルを開く
	std::ifstream ifs(targetFile);

	// 開けなかった場合
	if (!ifs.is_open()) return{};

	// 1行ずつ読み取る
	std::string line{};

	while (std::getline(ifs, line))
	{
		// クラス名宣言の行を検出
		std::regex re(R"(\bclass\s([a-zA-Z_][a-zA-Z0-9_:]*)\s*(?::\s*(public|private|protected)\s([^{]+))?)");

		std::smatch matches{};

		if (std::regex_match(line, matches, re))
		{
			for (auto& match : matches)
			{
				std::cout << match << std::endl;
			}
		}
	}
}
