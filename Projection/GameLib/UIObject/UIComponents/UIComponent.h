//====================================================//
// ファイル名   : UIComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/11
//
// 概要 : UI用のコンポーネントの仲介クラス
//
// 更新履歴 :
// 2026/05/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "BaseUIComponent.h"
#include "UIComponentID.h"

//====================================================//
// クラス宣言
//====================================================//
template<typename T, UIComponentID ID, bool Main = true>
class UIComponent : public BaseUIComponent
{
public:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------
    static constexpr UIComponentID TYPE_ID = ID;
    static constexpr bool IS_MAIN = Main;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    UIComponent(UIObject* own)
        : BaseUIComponent(own, TYPE_ID, IS_MAIN)
    {
    };
    virtual ~UIComponent() = default;
};