//====================================================//
// ファイル名   : BaseGraphic.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/23
//
// 概要 : グラフィックコンポーネントの基底クラス
//
// 更新履歴 :
// 2026/05/23 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../BaseUIComponent.h"
#include "../UIComponentID.h"
#include "../RectTransform/RectTransform.h"

#include "GameLib/Common/Renderer/Renderer.h"

//====================================================//
// クラス宣言
//====================================================//
class BaseGraphic : public BaseUIComponent
{
    // ----------------------------------------------------
    // 定数宣言
    // ----------------------------------------------------
public:
    static constexpr UIComponentID TYPE_ID = UIComponentID::Graphic;
    static constexpr bool IS_MAIN = true;


private:
    RectTransform* m_pRectTransform;

    // 色
    int m_color;
    int m_mulColor;

    // 透明度
    float m_alpha;
public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    BaseGraphic(UIObject* own, UIComponentID ID, bool isMain);

    virtual ~BaseGraphic() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // 描画関数
    virtual void Draw(Renderer& renderer) = 0;

	RectTransform* GetRectTransform() const { return m_pRectTransform; }

    void SetColor(int color) { m_color = color; }
    void SetMulColor(int color) { m_mulColor = color; }
    void SetAlpha(float alpha) { m_alpha = alpha; }

protected:

    int GetColor() const { return m_color; }
    int GetMulColor() const { return m_mulColor; }
    float GetAlpha() const { return m_alpha; }
};