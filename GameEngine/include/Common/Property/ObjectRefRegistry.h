//====================================================//
// ファイル名   : ObjectRefRegistry.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/28
//
// 概要 : オブジェクト参照を管理するクラス
//
// 更新履歴 :
// 2026/08/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <functional>
#include <typeindex>
#include <unordered_map>

//====================================================//
// クラス宣言
//====================================================//
class ObjectRefRegistry
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// オブジェクトを読み込む関数マップ
	std::unordered_map<std::type_index, std::function<void(void*, const IAssetResolver&)>> m_resolveMap;

private:
	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ObjectRefRegistry() = default;
	~ObjectRefRegistry() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------
public:
	// シングルトン化
	static ObjectRefRegistry& Instance()
	{
		static ObjectRefRegistry instance;
		return instance;
	}

	template<class T>
	void Register()
	{
		// TypeIndexを取得
		auto idx = std::type_index(typeid(T));

		// 既にあればなにもしない
		if (m_assignMap.contains(idx)) return;

		// マップに追加
	}

	// 値を反映させる関数
	void Assign(std::type_index idx, void* ptr, const UnTypeHandle& handle) const { m_assignMap.at(idx)(ptr, handle); }
};
