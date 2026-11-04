/**
 * @file    ComponentRegister.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/26
 *
 * @brief   コンポーネントを各マネージャーへ登録するクラス
 */

#pragma once

#include <vector>
#include <unordered_map>

#include "Common/UUID.h"
#include "Components/ComponentBase.h"
#include "Components/Interface/IComponentListener.h"
#include "Components/Interface/IDebugRenderable.h"
#include "Components/Interface/IWindowSizeDependent.h"
#include "Components/ComponentTypeId.h"

namespace RHEngine
{
	class UpdatePipeline;
	class ComponentBasel;

	/**
	 * @brief コンポーネントのレジスタ
	 */
	class ComponentRegister
	{
	private:

		/** @brief 更新順を管理するパイプライン */
		UpdatePipeline* m_pPipeline;

		/** @brief コンポーネントの追加削除を通知されるコンポーネントのリスト */
		std::vector<IComponentListener*> m_listeners;

		/** @brief デバッグ描画を行うコンポーネントのリスト */
		std::vector<IDebugRenderable*> m_debugRenders;

		/** @brief スクリーンサイズ依存コンポーネントのリスト */
		std::vector<IWindowSizeDependent*> m_screenSizeDependents;

		/** @brief 全コンポーネントのタイプをキーとしたマップ */
		std::unordered_map<Component::TypeId, std::vector<ComponentBase*>> m_componentsMap;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pipeline パイプライン
		 */
		ComponentRegister(UpdatePipeline* pPipeline);

		~ComponentRegister() = default;

		/// @}

		/** @brief 各マネージャーへ登録する関数 */
		void RegisterComponent(ComponentBase* pComponent);

		/** @brief 各マネージャーから登録解除する関数 */
		void UnRegisterComponent(ComponentBase* pComponent);

		/** @brief デバッグ描画コンポーネントの全取得 */
		const std::vector<IDebugRenderable*>& GetDebugRenders() const { return m_debugRenders; }

		/**
		 * @brief 指定された型のコンポーネントをシーンから全て取得する
		 * @tparam T 取得する型
		 * @return 取得したコンポーネントリスト
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, T>>>
		std::vector<ComponentBase*>& GetAllComponents()
		{
			// IDを取得し返す
			return m_componentsMap[T::StaticTypeId()];
		}

		/**
		 * @brief シーンのコンポーネント全てから指定の型のコンポーネントを1つ取得する
		 * @tparam T 
		 * @return 取得したコンポーネント
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, T>>>
		ComponentBase* GetComponent()
		{
			// IDを取得し返す
			if (m_componentsMap[T::StaticTypeId()].size() > 0)
				return m_componentsMap[T::StaticTypeId()][0];

			return nullptr;
		}

		/**
		 * @brief UUIDからコンポーネントを取得する
		 * @tparam T 取得対象の型
		 * @param uuid 調べるUUID
		 * @return 取得したコンポーネント
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of_v<ComponentBase, T>>>
		T* GetComponentFromUUID(UUID uuid)
		{
			// TypeIdを取得
			auto id = T::StaticTypeId();

			// リストがあるか調べる
			auto it = m_componentsMap.find(id);

			// あれば
			if (it != m_componentsMap.end())
			{
				// 探索
				for (auto component : m_componentsMap[id])
				{
					// UUIDの一致チェック
					if (uuid == component->GetUUID())
					{
						return static_cast<T*>(component);
					}
				}
			}

			// 見つからなければnullptr
			return nullptr;
		}

		/**
		 * @brief スクリーンサイズ依存のコンポーネントを全て取得する
		 * @return 
		 */
		std::vector<IWindowSizeDependent*>& GetScreenSizeDependents() { return m_screenSizeDependents; }
	};
}	// namespace RHEngine
