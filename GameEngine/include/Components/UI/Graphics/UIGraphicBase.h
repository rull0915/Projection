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
#include "Components/UI/UIComponentBase.h"
#include "Components/UI/RectTransform/RectTransform.h"

#include "Common/Origin.h"

//====================================================//
// 前方宣言
//====================================================//
class Renderer;

//====================================================//
// クラス宣言
//====================================================//
class UIGraphicBase : public UIComponentBase
{
private:
	RectTransform* m_pRectTransform;

	// 色
	DirectX::SimpleMath::Color m_color;
	DirectX::SimpleMath::Color m_mulColor;

	// 透明度
	float m_alpha;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	UIGraphicBase(IComponentOwner* owner);

	virtual ~UIGraphicBase() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 描画関数
	virtual void Draw(Renderer& renderer) = 0;

	RectTransform* GetRectTransform() const { return m_pRectTransform; }

	void SetColor(DirectX::SimpleMath::Color color) { m_color = color; }
	void SetMulColor(DirectX::SimpleMath::Color color) { m_mulColor = color; }
	void SetAlpha(float alpha) { m_alpha = alpha; }

	// カテゴリをUIグラフィックに指定
	ComponentCategory GetCategory() const override
	{
		return Category::UIGraphic;
	}

protected:

	DirectX::SimpleMath::Color GetColor() const { return m_color; }
	DirectX::SimpleMath::Color GetMulColor() const { return m_mulColor; }
	float GetAlpha() const { return m_alpha; }
};
