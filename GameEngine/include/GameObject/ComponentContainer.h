/**
 * @file    ComponentContainer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/29
 *
 * @brief   コンポーネントを保持するクラス
 */

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>

#include "Components/ComponentBase.h"
#include "Components/World/Transform/Transform.h"
#include "Components/UI/RectTransform/RectTransform.h"
#include "Components/ComponentTypeId.h"
#include "System/UUIDRegistry.h"
#include "Components/ComponentInfo.h"

namespace RHEngine
{
	class Scene;
	class IComponentOwner;
	class GameTimer;

	/**
	 * @brief コンポーネントを保持する
	 */
	class ComponentContainer
	{
	private:

		/** @brief 自身を持つシーンのポインタ */
		Scene* m_pScene;

		/** @brief トランスフォーム本体 */
		std::unique_ptr<Transform> m_pTransform;

		/** @brief レクトトランスフォーム本体 */
		std::unique_ptr<RectTransform> m_pRectTransform;

		/** @brief 追加予約中のコンポーネント */
		std::vector<std::unique_ptr<ComponentBase>> m_addReserves;

		/** 
		 * @brief 削除予約中のコンポーネント 
		 * @details 削除対象の重複を防ぐためにunordered_setを使用
		 */
		std::unordered_set<ComponentBase*> m_pDestroyReserves;

		/** @brief 保持するコンポーネントのリスト */
		std::vector<std::unique_ptr<ComponentBase>> m_pComponents;

		/** @brief タイプをキーとしたコンポーネントのキャッシュ */
		std::unordered_map<Component::TypeId, std::vector<ComponentBase*>> m_componentsCache;

		/** @brief 所有者 */
		IComponentOwner* m_pOwner;

	public:

		/// @name ライフサイクル
		/// @{

		ComponentContainer(IComponentOwner* owner);

		~ComponentContainer() = default;

		/// @}

		/**
		 * @brief シーンをセットする 
		 */
		void SetScene(Scene* pScene) { m_pScene = pScene; }

		/**
		 * @brief 全コンポーネントのAwakeを呼ぶ
		 */
		void AwakeComponets();

		/**
		 * @brief 全コンポーネントの更新を行う
		 * @param gameTimer 
		 */
		void UpdateComponents(const GameTimer& gameTimer);

		/**
		 * @brief 全コンポーネントの遅延更新を行う
		 * @param gameTimer 
		 */
		void LateUpdateComponents(const GameTimer& gameTimer);

		/**
		 * @brief 自身がUIオブジェクトであるかどうか
		 */
		bool IsUI() const { return m_pRectTransform && !m_pTransform; }

		/**
		 * @brief 自身がWorldオブジェクトであるかどうか
		 */
		bool IsWorld() const { return m_pTransform && !m_pRectTransform; }

		/**
		 * @brief 有効なオブジェクトであるかどうか
		 * @details UI・Worldのどちらかであれば有効
		 */
		bool IsValid() const { return IsUI() ^ IsWorld(); }

		/**
		 * @brief まだ所属空間が未設定であることを調べる
		 */
		bool IsNotSet() const { return !m_pTransform && !m_pRectTransform; }

		/**
		 * @brief コンポーネントの所属空間と自身の所属空間が等しいか調べる
		 * @param space 調べるコンポーネントの所属空間
		 */
		bool IsCorrect(ComponentSpace space) const;

		/// @name コンポーネント操作関数
		/// @{

		/**
		 * @brief コンポーネントを取得する
		 * @param id 取得するコンポーネントのID
		 * @return 取得したコンポーネント
		 */
		ComponentBase* Get(Component::TypeId id) const;

		/**
		 * @brief コンポーネントを取得する
		 * @tparam T 取得するコンポーネントの型
		 * @return 取得したコンポーネント
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		T* Get() const
		{
			return static_cast<T*>(Get(T::StaticTypeId()));
		}

		/**
		 * @brief 指定したコンポーネントを全て取得する
		 * @param id 取得するコンポーネントのID
		 * @return 取得したコンポーネントリスト
		 */
		const std::vector<ComponentBase*>& Gets(Component::TypeId id) const;

		/**
		 * @brief 指定したコンポーネントを全て取得する
		 * @tparam T 取得するコンポーネントの型
		 * @return 取得したコンポーネントリスト
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		const std::vector<ComponentBase*>& Gets() const
		{
			return Gets(T::StaticTypeId());
		}

		/**
		 * @brief コンポーネントを追加する
		 * @tparam T 追加するコンポーネントの型
		 * @return 追加したコンポーネント
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		T* Add()
		{
			ComponentBase* add = nullptr;

			// トランスフォームの場合
			if constexpr (std::is_same_v<T, Transform>)
			{
				// まだオブジェクトタイプが未設定なら
				if (IsNotSet())
				{
					// Transformを生成
					m_pTransform = std::make_unique<Transform>(m_pOwner);

					// 初期化
					m_pTransform.get()->UpdateCache();

					add = m_pTransform.get();
				}

				// 既に確定していたら追加不可
				else return m_pTransform.get();
			}

			// RectTransformの場合
			else if constexpr (std::is_same_v<T, RectTransform>)
			{
				// まだオブジェクトタイプが未設定なら
				if (IsNotSet())
				{
					// RectTransformを生成
					m_pRectTransform = std::make_unique<RectTransform>(m_pOwner);

					// 初期化
					m_pRectTransform.get()->UpdateCache();
					
					add = m_pRectTransform.get();
				}

				// 既に確定していたら追加不可
				else return m_pRectTransform.get();
			}

			else
			{
				// ユニークポインタを生成
				auto comp = std::make_unique<T>(m_pOwner);
				T* ptr = comp.get();

				// オブジェクトの所属空間と追加コンポーネントの所属空間が同じか調べる
				if (IsCorrect(comp->SPACE))
				{
					// 配列に追加
					m_addReserves.push_back(std::move(comp));

					add = ptr;
				}
			}

			if (add)
			{
				// UUIDを設定
				add->SetUUID(UUIDRegistry::Instance().GenerateUUID());

				// キャッシュに追加
				RegistComponentToCache(add);

				// シーンに登録
				RegisterComponentToScene(add);
			}

			return static_cast<T*>(add);
		}

		/**
		 * @brief コンポーネントを削除する
		 * @tparam T 削除したいコンポーネントの型
		 * @param component 削除するコンポーネント
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void Remove(T* component)
		{
			m_pDestroyReserves.insert(component);
		}

		/**
		 * @brief コンポーネントを削除する
		 * @tparam T 削除したいコンポーネントの型
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void Remove()
		{
			// リストを取得
			auto it = m_componentsCache.find(T::StaticTypeId());

			// あれば
			if (it != m_componentsCache.end() && it->second.size() > 0)
			{
				// 最初の要素を削除リストに追加
				m_pDestroyReserves.insert(it->second[0]);
			}
		}

		/**
		 * @brief 指定したコンポーネントをすべて削除する
		 * @tparam T 削除するコンポーネントの型
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		void Removes()
		{
			// リストを取得
			auto it = m_componentsCache.find(T::StaticTypeId());

			// あれば
			if (it != m_componentsCache.end())
			{
				// 全て削除リストに追加
				for (auto& comp : it->second)
				{
					m_pDestroyReserves.insert(comp);
				}
			}
		}

		/**
		 * @brief コンポーネントを削除する
		 * @param id 削除するコンポーネントのID
		 */
		void Remove(Component::TypeId id);

		/**
		 * @brief 指定したコンポーネントを全て取得する
		 * @param id 削除するコンポーネントのID
		 */
		void Removes(Component::TypeId id);

		/**
		 * @brief 追加予約済みのコンポーネントを登録する
		 */
		void RegisterComponents();

		/**
		 * @brief 削除予約済みのコンポーネントを削除する
		 */
		void RemoveRegistered();

	private:

		/**
		 * @brief シーンにコンポーネントを登録する
		 * @param pComponent 登録するコンポーネント
		 */
		void RegisterComponentToScene(ComponentBase* pComponent);

		/**
		 * @brief シーンからコンポーネント登録解除する
		 * @param pComponent 登録解除するコンポーネント
		 */
		void UnRegisterComponentToScene(ComponentBase* pComponent);

		/**
		 * @brief キャッシュにコンポーネントを登録する
		 * @param pComponent 登録するコンポーネント
		 */
		void RegistComponentToCache(ComponentBase* pComponent);

		/**
		 * @brief キャッシュからコンポーネントを登録解除する
		 * @param pComponent 登録解除するコンポーネント
		 */
		void UnRegistComponentToCache(ComponentBase* pComponent);
	};
} // namespace RHEngine
