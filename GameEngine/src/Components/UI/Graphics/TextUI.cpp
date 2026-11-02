/**
 * @file    TextUI.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/26
 */

#include "pch.h"
#include "Components/UI/Graphics/TextUI.h"

#include "Renderer/Renderer.h"
#include "Assets/Managers/AssetManager.h"
#include "Common/CastString.h"
#include "Components/UI/RectTransform/RectTransform.h"

namespace RHEngine
{
	void TextUI::Draw(Renderer& renderer)
	{
		// AssetManagerがなければ何もしない
		if (!m_assetManager) return;

		// フォントを取得
		Font* pFont = m_assetManager->Get<Font>(m_fontHandle);

		// 出来なければ何もしない
		if (!pFont) return;

		RectTransform* pRecrTransform = GetRectTransform();

		// ピボットのピクセル座標
		DirectX::SimpleMath::Vector2 pivotPixelPos = pRecrTransform->GetPivotPixelPos();

		DirectX::SimpleMath::Vector2 pivot = pRecrTransform->GetPivot();
		DirectX::SimpleMath::Vector2 disPivot = DirectX::SimpleMath::Vector2{ 1 - pivot.x, 1 - pivot.y };

		DirectX::SimpleMath::Vector2 size = pRecrTransform->GetSize();
		DirectX::SimpleMath::Vector2 scale = pRecrTransform->GetWorldScale();

		float defaultFontSize = pFont->GetFontSize();

		float fontScale = m_fontSize / defaultFontSize;

		// 描画位置を算出
		DirectX::SimpleMath::Vector2 drawPos =
			pRecrTransform->GetLUPixelPos() +
			pRecrTransform->GetRight() * size.x * scale.x * m_origin.x +
			pRecrTransform->GetUp() * size.y * scale.y * m_origin.y;

		std::wstring s = CastString::Utf8ToWide(m_text);

		renderer.Draw().Text().Draw(
			pFont->Get(), s,
			drawPos, scale * fontScale, pRecrTransform->GetWorldRotation(), m_origin, GetColor() * GetMulColor()
		);
	}
}	// namespace RHEngine
