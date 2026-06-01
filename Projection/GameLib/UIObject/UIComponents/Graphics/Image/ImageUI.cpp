//====================================================//
// ファイル名  : ImageUI.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/23
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ImageUI.h"

#include "GameLib/GameMath/Color/ColorLib.h"

//====================================================//
// 関数の実体宣言
//====================================================//

/// <summary>
/// 描画関数
/// </summary>
/// <param name="renderer"></param>
void ImageUI::Draw(Renderer& renderer)
{
	RectTransform* transform = GetRectTransform();

	// ピボットのピクセル座標
	DirectX::SimpleMath::Vector2 pivotPixelPos = transform->GetPivotPixelPos();

	DirectX::SimpleMath::Vector2 pivot = transform->GetPivot();
	DirectX::SimpleMath::Vector2 disPivot = DirectX::SimpleMath::Vector2{ 1 - pivot.x, 1 - pivot.y };

	DirectX::SimpleMath::Vector2 size = transform->GetSize();

	// 透明度を指定
	renderer.SetAlpha(GetAlpha());

	// 描画
	renderer.Draw().Sprite().
		Rect(pivotPixelPos - size * pivot, pivotPixelPos + size * disPivot).	// 矩形指定
		Rotate(transform->GetWorldRotation()).									// 回転
		Extend(transform->GetWorldScale()).						// 拡大縮小
		Origin(pivot).
		Execute(m_pTexture, MyColor::MultiplyColor(GetColor(), GetMulColor()));							// 描画呼び出し

	// 描画後はアルファを元に戻す
	renderer.SetAlpha(1.0f);
}
