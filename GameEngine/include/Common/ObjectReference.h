//====================================================//
// ファイル名   : ObjectReference.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/28
//
// 概要 : オブジェクトの参照を扱うクラス
//
// 更新履歴 :
// 2026/08/28 新規作成
//====================================================//

#pragma once

#include "Common/UUID.h"
#include <functional>

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class RefBase
	{
	public:
		using DestroyCallBack = std::function<void(RefBase*)>;

	private:
		// UUID
		UUID m_uuid;

		// 削除時に呼ばれるコールバック関数
		DestroyCallBack m_destroyCallback;

	public:
		virtual ~RefBase()
		{
			// コールバックが設定されていれば呼び出す
			if (m_destroyCallback) m_destroyCallback(this);
		}

		// UUID取得関数
		UUID GetUUID() const
		{
			return m_uuid;
		}

		// UUIDセット関数
		void SetUUID(UUID uuid)
		{
			m_uuid = uuid;
		}

		// コールバックセット関数
		void SetDestroyCallBack(DestroyCallBack callback)
		{
			m_destroyCallback = callback;
		}

		// 無効化関数
		virtual void Invalidate() = 0;
	};

	template<typename T>
	class Ref : public RefBase
	{
	public:
		// 型識別用
		using value_type = T;

	private:
		// 参照先
		T* m_obj = nullptr;

	public:

		// 取得関数
		T* Get() const
		{
			return m_obj;
		}

		// 各演算子オーバーロード

		// ->演算子をオーバーロードすることで Ref<T> -> でポインタのように扱えるようにします
		T* operator->() const
		{
			return m_obj;
		}

		// boolをオーバーロードすることで if (Ref) でnullチェックを行えるようにします
		operator bool() const
		{
			return m_obj != nullptr;
		}

		// 無効化関数
		void Invalidate() override
		{
			m_obj = nullptr;
		}
	};

	// 型がRefであるかを調べるための構造体

	// Ref以外ならfalse
	template<typename>
	struct IsRef : std::false_type {};

	// Refならtrue
	template<typename U>
	struct IsRef<Ref<U>> : std::true_type {};

	// 結果を直接返すインスタンス
	template<typename T>
	inline constexpr bool IsRef_v = IsHandle<T>::value;
}
