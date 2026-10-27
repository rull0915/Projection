/**
 * @file    IComponentOwner.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/28
 *
 * @brief   コンポーネントがデフォルトで呼び出せるメソッドのみをまとめたインターフェース
 */

#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include "Components/ComponentTypeId.h"

namespace RHEngine
{
	class ComponentBase;

	/**
	 * @brief コンポーネントがデフォルトで呼び出せるメソッドのみをまとめたインターフェース
	 */
	class IComponentOwner
	{
	public:

		/// @name ライフサイクル
		/// @{

		virtual ~IComponentOwner() = default;

		/// @}

		// 自身の削除
		virtual void Destroy() = 0;

		/**
		 * @brief コンポーネントを取得する
		 * @tparam T 取得したいコンポーネントの型
		 * @return 取得したコンポーネント
		 */
		template<typename T>
		T* GetComponent()
		{
			return static_cast<T*>(GetComponentRaw(T::StaticTypeId()));
		}

		/**
		 * @brief 指定した型のコンポーネントをすべて取得する
		 * @tparam T 取得したいコンポーネントの型
		 * @return コンポーネントリスト
		 */
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		const std::vector<ComponentBase*>& GetComponents()
		{
			return GetComponentsRaw(T::StaticTypeId());
		}

		/// @name ゲッター
		/// @{
		
		/** @brief タグ名 */
		virtual const std::string& GetTag() const = 0;

		/** @brief オブジェクト名 */
		virtual const std::string& GetName() const = 0;
		
		/** @brief アクティブフラグ */
		virtual bool IsActive() const = 0;

		/// @}

		/// @name セッター
		/// @{

		/** @brief タグ名 */
		virtual void SetTag(const std::string& tag) = 0;

		/** @brief オブジェクト名 */
		virtual void SetName(const std::string& name) = 0;

		/// @}

	private:

		/**
		 * @brief コンポーネントを取得する
		 * @param id 取得するコンポーネントのID
		 * @return 取得したコンポーネント
		 */
		virtual ComponentBase* GetComponentRaw(
			Component::TypeId id
		) const = 0;

		/**
		 * @brief 指定したコンポーネントを全て取得する
		 * @param id 取得するコンポーネントのID
		 * @return 取得したコンポーネントリスト
		 */
		virtual const std::vector<ComponentBase*>& GetComponentsRaw(
			Component::TypeId id
		) const = 0;
	};
}	// namespace RHEngine
