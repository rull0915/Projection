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

	template<typename T, typename = std::enable_if_t<std::is_base_of_v<AssetBase, T>>>
	struct Handle;

	// タイプ識別なしのハンドル
	struct UnTypeHandle
	{
		uint32_t index;
		uint32_t generation;

		template<typename T>
		Handle<T> As() const;
	};

	// Assetに対応するハンドル
	template<typename T, typename E>
	struct Handle
	{
	public:
		// 型識別用
		using value_type = T;

		uint32_t index;
		uint32_t generation;

		UnTypeHandle GetUnTypeHandle();
	};

	//------------------------------
	// 実装
	//------------------------------

	template<typename T>
	Handle<T> UnTypeHandle::As() const
	{
		return { index, generation };
	}

	template<typename T, typename E>
	UnTypeHandle Handle<T, E>::GetUnTypeHandle()
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
