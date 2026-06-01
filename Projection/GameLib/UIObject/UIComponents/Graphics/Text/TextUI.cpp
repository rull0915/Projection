//====================================================//
// ファイル名  : TextUI.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/26
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "TextUI.h"
#include "GameLib/Resources/ResourceManager.h"

#include "GameLib/GameMath/Color/ColorLib.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void TextUI::Draw(Renderer& renderer)
{
	if (!m_pFont) return;

	RectTransform* transform = GetRectTransform();

	// ピボットのピクセル座標
	DirectX::SimpleMath::Vector2 pivotPixelPos = transform->GetPivotPixelPos();

	DirectX::SimpleMath::Vector2 pivot = transform->GetPivot();
	DirectX::SimpleMath::Vector2 disPivot = DirectX::SimpleMath::Vector2{ 1 - pivot.x, 1 - pivot.y };

	DirectX::SimpleMath::Vector2 size = transform->GetSize();
	DirectX::SimpleMath::Vector2 scale = transform->GetWorldScale();

	float defaultFontSize = ResourceManager::Instance().GetFontSize(m_pFont);

	float fontScale = m_fontSize / defaultFontSize;

	// 透明度を指定
	renderer.SetAlpha(GetAlpha());

	// 描画位置を算出
	DirectX::SimpleMath::Vector2 ratio = Origin::GetPosition(m_origin);

	DirectX::SimpleMath::Vector2 drawPos =
		transform->GetLUPixelPos() +
		transform->GetRight() * size.x * scale.x * ratio.x +
		transform->GetUp() * size.y * scale.y * ratio.y;

	// 描画
	renderer.Draw().Text().
		Rotate(transform->GetWorldRotation()).								// 回転
		Extend(scale * fontScale).									// 拡大縮小
		Origin(ratio).
		Execute(m_pFont, m_text.c_str(), drawPos, 
			MyColor::MultiplyColor(GetColor(), GetMulColor()));							// 描画呼び出し

	// 描画後はアルファを元に戻す
	renderer.SetAlpha(1.0f);
}
