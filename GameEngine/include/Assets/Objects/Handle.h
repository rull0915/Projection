/**
 * @file    Handle.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/26
 *
 * @brief   Assetに対応するHandle構造体
 */

#pragma once

#include <type_traits>
#include <cstdint>

namespace RHEngine
{
	template<typename T>
	struct Handle;
	class AssetBase;
	struct UnTypeHandle;

	/**
	 * @brief タイプ識別なしのアセットハンドル
	 */
	struct UnTypeHandle
	{
		/** @brief 自身のインデックス (AssetRegistryが持つSlotリストで使用) */
		uint32_t index = 0;

		/** @brief 自身の世代 (AssetRegistryが持つSlotリストで使用) */
		uint32_t generation = 0;

		/**
		 * @brief タイプ識別ありのハンドルへ変換する
		 * @tparam T アセット型
		 * @return タイプ識別ありのハンドル
		 */
		template<typename T>
		Handle<T> As() const;

		/**
		 * @brief 等価演算子オーバーロード
		 * @param handle 比較対象のハンドル
		 * @return 等価かどうか
		 */
		bool operator==(const UnTypeHandle& handle) const
		{
			return index == handle.index && generation == handle.generation;
		}
		
		/**
		 * @brief 有効のハンドルかどうか
		 * @return 有効かどうか
		 */
		bool IsValid() const
		{
			return index != 0 || generation != 0;
		}
	};

	/**
	 * @brief アセットに対応するハンドル
	 * @tparam T アセット型
	 */
	template<typename T>
	struct Handle
	{
		// 型識別用
		using value_type = T;

		/** @brief 自身のインデックス (AssetRegistryが持つSlotリストで使用) */
		uint32_t index = 0;

		/** @brief 自身の世代 (AssetRegistryが持つSlotリストで使用) */
		uint32_t generation = 0;

		/**
		 * @brief タイプ識別なしのハンドルへ変換する
		 * @return タイプ識別なしハンドル
		 */
		UnTypeHandle GetUnTypeHandle() const;

		/**
		 * @brief 等価演算子オーバーロード
		 * @param handle 比較対象のハンドル
		 * @return 等価かどうか
		 */
		bool operator==(const Handle<T>& handle) const
		{
			return index == handle.index && generation == handle.generation;
		}

		/**
		 * @brief 有効ハンドルかどうか
		 * @return 有効かどうか
		 */
		bool IsValid() const
		{
			return index != 0 || generation != 0;
		}
	};

	/**
	 * @brief タイプ識別なしエラーハンドル
	 */
	static constexpr UnTypeHandle ERROR_UNTYPE_HANDLE = { 0, 0 };

	/**
	 * @brief エラーハンドル
	 * @tparam T アセット型
	 */
	template<typename T>
	static constexpr Handle<T> ERROR_HANDLE = { 0, 0 };

	template<typename T>
	Handle<T> UnTypeHandle::As() const
	{
		return { index, generation };
	}

	template<typename T>
	UnTypeHandle Handle<T>::GetUnTypeHandle() const
	{
		return { index, generation };
	}

	/**
	 * @brief 型がハンドルかどうかを調べる
	 * @tparam  調べる対象の型
	 */
	template<typename>
	struct IsHandle : std::false_type {};

	/**
	 * @brief 型がハンドルかどうかを調べる
	 * @tparam U 調べる対象の型
	 */
	template<typename U>
	struct IsHandle<Handle<U>> : std::true_type {};

	/**
	 * @brief 型がハンドルかどうかを調べる (結果を直接返す)
	 * @tparam T 調べる対象の型
	 */
	template<typename T>
	inline constexpr bool IsHandle_v = IsHandle<T>::value;
}	// namespace RHEngine
