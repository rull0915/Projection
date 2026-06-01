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
#include <type_traits>

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

    // ------ Add ------ //
    template<typename T>
    T* AddComponent()
    {

    }

    // --------- 内部実装 ---------- //
private:
    virtual BaseComponent* GetComponentRaw(
        ComponentID id,
        bool isMain) = 0;
    //virtual BaseComponent* AddComponentRaw(
    //    ComponentID id,
    //    bool isMain) = 0;
    virtual bool HasComponentRaw(
        ComponentID id,
        bool isMain) = 0;
    //virtual void RemoveComponentRaw(
    //    ComponentID id,
    //    bool isMain) = 0;

    // 削除
    virtual void Destroy() = 0;
};