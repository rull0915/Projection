/**
 * @file    ObjectReference.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/28
 *
 * @brief   プロパティオブジェクトの参照を扱うクラス
 */

#pragma once

#include <functional>

#include "Common/UUID.h"

namespace RHEngine
{
	class PropertyObject;

	/**
	 * @brief プロパティオブジェクト参照用ラッパーの基底
	 */
	class RefBase
	{
	public:
		using DestroyCallBack = std::function<void(RefBase*)>;

	private:
		/** @brief 参照するオブジェクトのID */
		UUID m_uuid = UUID_NONE;

		/** @brief 削除時に呼ばれるコールバック関数 */
		DestroyCallBack m_destroyCallback;

	public:

		/// @name ライフサイクル
		/// @{
		 
		RefBase() = default;

		virtual ~RefBase()
		{
			// コールバックが設定されていれば呼び出す
			if (m_destroyCallback) m_destroyCallback(this);
		}

		/// @}

		/**
		 * @brief 参照先のポインタを変更する
		 * @param obj 変更先のポインタ
		 * @return 成功したかどうか
		 */
		virtual bool SetObj(PropertyObject* obj) = 0;

		/**
		 * @brief 参照しているポインタを返す
		 * @return 参照先
		 */
		virtual PropertyObject* GetPropertyObject() const = 0;

		/**
		 * @brief 参照を無効化する
		 * @details 派生クラス側で参照しているポインタをnullにするなどの処理を記述する
		 */
		virtual void Invalidate() = 0;

		/**
		 * @brief 参照しているオブジェクトのUUIDを取得する
		 */
		UUID GetUUID() const
		{
			return m_uuid;
		}

		/**
		 * @brief 参照先のUUIDを変更する
		 */
		void SetUUID(UUID uuid)
		{
			m_uuid = uuid;
		}

		/**
		 * @brief 削除時のコールバック関数をセットする
		 */
		void SetDestroyCallBack(DestroyCallBack callback)
		{
			m_destroyCallback = callback;
		}
	};

	/**
	 * @brief プロパティオブジェクト派生の参照を扱うラッパー
	 * @tparam T 参照するオブジェクトの型
	 * @tparam  プロパティオブジェクトの派生クラスのみを扱うように制限
	 */
	template<typename T, typename = std::enable_if<std::is_base_of_v<PropertyObject, T>>>
	class Ref : public RefBase
	{
	public:
		using value_type = T;

	private:
		/** @brief 参照先のアドレス */
		T* m_obj = nullptr;

	public:

		/**
		 * @brief ->演算子オーバーロード
		 * @details ->をオーバーロードすることで通常のポインタと同じように扱うことが出来るようにする
		 * @return 
		 */
		T* operator->() const
		{
			return m_obj;
		}

		/**
		 * @brief boolオーバーロード
		 * @details boolをオーバーロードすることで、if (Ref<T>) のようにNULLチェックを行えるようにする
		 */
		operator bool() const
		{
			return m_obj != nullptr;
		}
		
		/**
		 * @brief 参照先を取得する
		 * @return 参照しているポインタ
		 */
		T* Get() const
		{
			return m_obj;
		}

		/**
		 * @brief 参照先を取得する (基底のポインタとして取得する)
		 * @return 参照しているポインタ
		 */
		PropertyObject* GetPropertyObject() const override
		{
			return m_obj;
		}

		/**
		 * @brief 参照先を変更する
		 * @param obj 変更先のポインタ
		 * @return 成功したかどうか
		 */
		bool SetObj(PropertyObject* obj) override
		{
			// 型が一致すれば
			if (T* ptr = dynamic_cast<T*>(obj))
			{
				// セット
				m_obj = ptr;

				return true;
			}

			return false;
		}

		/**
		 * @brief 参照を無効化する
		 */
		void Invalidate() override
		{
			m_obj = nullptr;

			SetDestroyCallBack(nullptr);
		}
	};

	/**
	 * @brief 型がRef型かどうか調べる
	 */
	template<typename>
	struct IsRef : std::false_type {};

	/**
	 * @brief 型がRef型かどうか調べる
	 */	template<typename U>
	struct IsRef<Ref<U>> : std::true_type {};

	/**
	 * @brief 型がRef型かどうか調べる (結果を直接返すインスタンス)
	 */
	template<typename T>
	inline constexpr bool IsRef_v = IsRef<T>::value;
}	// namespace RHEngine
