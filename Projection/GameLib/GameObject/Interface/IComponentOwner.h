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
#include "../Components/ComponentID.h"
#include <vector>

//====================================================//
// 前方宣言
//====================================================//
class BaseComponent;

//====================================================//
// クラス宣言
//====================================================//
class IComponentOwner
{
    // --------- 公開関数 ---------- //
public:

    virtual ~IComponentOwner() = default;

    // ------ Get ------ //
	template<typename T>
    T* GetComponent()
    {
        return static_cast<T*>(
            GetComponentRaw(
                T::TYPE_ID,
                T::IS_MAIN));
    }
    template<typename T>
    const T* GetComponent() const
    {
    return static_cast<const T*>(
            GetComponentRaw(
                T::TYPE_ID,
                T::IS_MAIN));
    }

	// 複数取得
	template<typename T, typename = std::enable_if_t<std::is_base_of<BaseComponent, T>::value>>
    void GetComponents(std::vector<T*>& out)
    {
		std::vector<BaseComponent*> components;
		GetComponentsRaw(
			T::TYPE_ID, T::IS_MAIN, components);

        out.clear();
		for (auto& component : components)
		{
			out.push_back(static_cast<T*>(component));
		}
    }
    
    // ------ Has ------ //
	template<typename T>
	bool HasComponent() const
	{
		return HasComponentRaw(
			T::TYPE_ID,
			T::IS_MAIN);
	}

    // --------- 内部実装 ---------- //
private:
    virtual BaseComponent* GetComponentRaw(
        ComponentID id,
        bool isMain) = 0;
    virtual bool HasComponentRaw(
        ComponentID id,
        bool isMain) = 0;
	virtual void GetComponentsRaw(
		ComponentID id,
		bool isMain,
		std::vector<BaseComponent*>& out) = 0;

    // 削除
    virtual void Destroy() = 0;
};