//====================================================//
// ファイル名   : IComponentOwner.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/28
//
// 概要 : コンポーネントが呼び出せるメソッドのみをまとめたインターフェースクラス
//
// 更新履歴 :
// 2026/05/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include "Components/ComponentTypeId.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class ComponentBase;

	//====================================================//
	// クラス宣言
	//====================================================//
	class IComponentOwner
	{
		// ============ 公開関数 ============ //
	public:

		virtual ~IComponentOwner() = default;

		// タグ
		virtual const std::string& GetTag() const = 0;
		virtual void SetTag(const std::string& tag) = 0;

		// オブジェクト名
		virtual const std::string& GetName() const = 0;
		virtual void SetName(const std::string& name) = 0;

		// アクティブ
		virtual bool IsActive() const = 0;

		// 自身の削除
		virtual void Destroy() = 0;

		// -------- Component関連 -------- //

		// ------ Get ------ //
		template<typename T>
		T* GetComponent()
		{
			return static_cast<T*>(GetComponentRaw(T::StaticTypeId()));
		}

		// 複数取得
		template<typename T, typename = std::enable_if_t<std::is_base_of<ComponentBase, T>::value>>
		const std::vector<ComponentBase*>& GetComponents()
		{
			return GetComponentsRaw(T::StaticTypeId());
		}

		// --------- 内部実装 ---------- //
	private:

		// 派生クラスで実装させる用の関数

		// 取得
		virtual ComponentBase* GetComponentRaw(
			Component::TypeId id
		) const = 0;

		// 複数取得
		virtual const std::vector<ComponentBase*>& GetComponentsRaw(
			Component::TypeId id
		) const = 0;
	};
} // namespace REngine
