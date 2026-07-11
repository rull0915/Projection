//====================================================//
// ファイル名   : PrefabManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/08
//
// 概要 : プレハブを管理するクラス
//
// 更新履歴 :
// 2026/07/08 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <unordered_map>
#include <string>

class GameObject;

//====================================================//
// クラス宣言
//====================================================//
class PrefabManager
{
private:
	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// リスト
	std::unordered_map<std::string, std::wstring> m_prefabList;

private:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	PrefabManager() = default;
	~PrefabManager() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------
public:

	// インスタンス
	static PrefabManager& Instance()
	{
		static PrefabManager instance;
		return instance;
	}

	// 追加
	void AddPrefab(const std::string& key, const std::wstring& path);

	// 取得
	const std::wstring GetPrefab(const std::string& key);

	// 読み込み関数
	void LoadPrefab(const std::string& key, GameObject* obj);

	// 全取得
	const std::unordered_map<std::string, std::wstring>& GetAllObjects() const { return m_prefabList; }
};
