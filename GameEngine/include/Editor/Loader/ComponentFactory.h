/**
 * @file    ComponentFactory.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/30
 *
 * @brief   コンポーネントの作成を担うクラス
 */

#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <utility>

#include "Components/ComponentBase.h"
#include "Components/ComponentInfo.h"

namespace RHEngine
{
	class GameObject;

	/**
	 * @brief コンポーネントの作成を担う
	 */
	class ComponentFactory
	{
		friend class InspectorWindow;

	private:
		using CreateFunc = std::function<ComponentBase* (GameObject*)>;

		/**
		 * @brief コンポーネント名をキーとした生成関数マップ
		 */
		static inline std::unordered_map<std::string, std::pair<ComponentInfo, CreateFunc>> m_creatorMap;

	public:

		/**
		 * @brief コンポーネントを登録する
		 * @param name コンポーネント名
		 * @param space コンポーネントの所属空間
		 * @param func 生成関数オブジェクト
		 */
		static void Register(const std::string& name, ComponentInfo space, CreateFunc func);

		/**
		 * @brief 作成する
		 * @param name コンポーネント名
		 * @param pOwner 所有者のポインタ
		 * @return 作成したコンポーネント
		 */
		static ComponentBase* Create(const std::string& name, GameObject* pOwner);
	};
} // namespace RHEngine
