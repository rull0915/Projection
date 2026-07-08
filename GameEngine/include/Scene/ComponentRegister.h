//====================================================//
// ファイル名   : ComponentRegister.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/26
//
// 概要 : コンポーネントを各マネージャーへ登録するクラス
//
// 更新履歴 :
// 2026/06/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <unordered_map>
#include "Components/ComponentBase.h"

#include "Components/Interface/IComponentListener.h"

//====================================================//
// 前方宣言
//====================================================//
class UpdatePipeline;
class ComponentBasel;

//====================================================//
// クラス宣言
//====================================================//
class ComponentRegister
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	UpdatePipeline* m_pPipeline;

	// コンポーネントを読み取るリスト
	std::vector<IComponentListener*> m_listeners;

	// 全コンポーネントのマップ
	std::unordered_map<unsigned int, std::vector<ComponentBase*>> m_componentsMap;

	// マップ(カテゴリ版)
	std::unordered_map<ComponentCategory, std::vector<ComponentBase*>> m_categoriesMap;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ComponentRegister(UpdatePipeline* pipeline);
	~ComponentRegister() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 各マネージャーへ登録する関数
	void RegisterComponent(ComponentBase* component);

	// 各マネージャーから登録解除する関数
	void UnRegisterComponent(ComponentBase* component);

	// 全コンポーネントの取得関数
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, T>>>
	std::vector<ComponentBase*>& GetAllComponents()
	{
		// IDを取得し返す
		return m_componentsMap[TypeIDGenerator::GetID<T>()];
	}

	// 1コンポーネントの取得関数
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, T>>>
	ComponentBase* GetComponent()
	{
		// IDを取得し返す
		if (m_componentsMap[TypeIDGenerator::GetID<T>()].size() > 0)
		return m_componentsMap[TypeIDGenerator::GetID<T>()][0];

		return nullptr;
	}

	// カテゴリ指定
	std::vector<ComponentBase*>& GetAllComponentsWithCategory(ComponentCategory category)
	{
		// 返す
		return m_categoriesMap[category];
	}

	// カテゴリ指定
	ComponentBase* GetComponentWithCategory(ComponentCategory category)
	{
		// 返す
		if (m_componentsMap[category].size() > 0)
			return m_componentsMap[category][0];

		return nullptr;
	}
};
