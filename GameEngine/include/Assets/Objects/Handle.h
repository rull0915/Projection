//====================================================//
// ファイル名   : Handle.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/07/26
//
// 概要 : Assetに対応するHandle構造体
//
// 更新履歴 :
// 2026/07/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <type_traits>

namespace REngine
{
	// 前方宣言
	class AssetBase;

	struct UnTypeHandle;

	template<typename T>
	struct Handle;

	// タイプ識別なしのハンドル
	struct UnTypeHandle
	{
		uint32_t index = 0;
		uint32_t generation = 0;

		template<typename T>
		Handle<T> As() const;

		bool operator==(const UnTypeHandle& handle) const
		{
			return index == handle.index && generation == handle.generation;
		}
	};

	// Assetに対応するハンドル
	template<typename T>
	struct Handle
	{
	public:
		// 型識別用
		using value_type = T;

		uint32_t index = 0;
		uint32_t generation = 0;

		UnTypeHandle GetUnTypeHandle() const;

		bool operator==(const Handle<T>& handle) const
		{
			return index == handle.index && generation == handle.generation;
		}
	};

	// エラーハンドル
	static constexpr UnTypeHandle ERROR_UNTYPE_HANDLE = { 0, 0 };

	template<typename T>
	static constexpr Handle<T> ERROR_HANDLE = { 0, 0 };

	//------------------------------
	// 実装
	//------------------------------

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

	// 型がHandleであるかを調べるための構造体

	// Handle以外ならfalse
	template<typename>
	struct IsHandle : std::false_type {};

	// Handleならtrue
	template<typename U>
	struct IsHandle<Handle<U>> : std::true_type {};

	// 結果を直接返すインスタンス
	template<typename T>
	inline constexpr bool IsHandle_v = IsHandle<T>::value;
}
