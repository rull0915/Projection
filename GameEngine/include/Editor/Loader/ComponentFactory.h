//====================================================//
// ファイル名   : ComponentFactory.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/30
//
// 概要 : コンポーネントの作成を担うクラス
//
// 更新履歴 :
// 2026/06/30 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <functional>
#include <map>
#include <string>

#include "Components/ComponentBase.h"

namespace REngine
{
	class GameObject;

	//====================================================//
	// クラス宣言
	//====================================================//
	class ComponentFactory
	{
		// インスペクターをフレンドに指定
		friend class InspectorWindow;

	private:
		// 別名宣言
		using CreateFunc = std::function<ComponentBase* (GameObject*)>;

		// 関数マップ
		static inline std::map<std::string, std::pair<ComponentInfo, CreateFunc>> m_creatorMap;

	public:

		// 登録関数
		static void Register(const std::string& name, ComponentInfo space, CreateFunc func);

		// 作成を実行
		static ComponentBase* Create(const std::string& id, GameObject* owner);
	};
} // namespace REngine
