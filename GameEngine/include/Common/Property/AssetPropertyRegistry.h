/**
 * @file    AssetPropertyRegistry.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   プロパティとしてのアセットを管理するクラス
 */

#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>

#include "Assets/Objects/Handle.h"
#include "Assets/Managers/IAssetResolver.h"

namespace RHEngine
{
	/**
	 * @brief プロパティとしてのアセットを管理する
	 */
	class AssetPropertyRegistry
	{
	private:

		/** @brief 値の変更を反映させる関数マップ */
		std::unordered_map<std::type_index, std::function<void(void*, const UnTypeHandle&)>> m_assignMap;

		/** @brief ハンドルからUUIDを取得する関数マップ */
		std::unordered_map<std::type_index, std::function<UUID(void*, const IAssetResolver&)>> m_getUUIDMap;

	private:
		/// @name ライフサイクル
		/// @{

		AssetPropertyRegistry() = default;

		~AssetPropertyRegistry() = default;

		/// @}

	public:
		/**
		 * @brief インスタンス取得 (シングルトンパターン)
		 * @return 静的なインスタンス
		 */
		static AssetPropertyRegistry& Instance()
		{
			static AssetPropertyRegistry instance;
			return instance;
		}

		/**
		 * @brief アセットを登録する
		 * @tparam T アセットの型
		 */
		template<class T>
		void Register()
		{
			// TypeIndexを取得
			auto idx = std::type_index(typeid(T));

			// 既にあればなにもしない
			if (m_assignMap.contains(idx)) return;

			// マップに追加
			m_assignMap[idx] =
				[](void* property, const UnTypeHandle& handle)
				{
					*static_cast<Handle<T>*>(property) = handle.As<T>();
				};

			m_getUUIDMap[idx] =
				[](void* property, const IAssetResolver& resolver)
				{
					return resolver.GetUUID(static_cast<Handle<T>*>(property)->GetUnTypeHandle());
				};
		}
		
		/**
		 * @brief ハンドルを変更する
		 * @param idx 変更するアセット型のtype_index
		 * @param ptr アセットハンドルのアドレス
		 * @param handle 変更後のハンドル
		 */
		void Assign(std::type_index idx, void* ptr, const UnTypeHandle& handle) const { m_assignMap.at(idx)(ptr, handle); }

		/**
		 * @brief UUIDを取得する
		 * @param idx 取得するアセット型のtype_index
		 * @param ptr アセットハンドルのアドレス
		 * @param resolver 
		 * @return 
		 */
		UUID GetUUID(std::type_index idx, void* ptr, const IAssetResolver& resolver) const { return m_getUUIDMap.at(idx)(ptr, resolver); }
	};
}	//namespace RHEngine
