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
#include "Components/UI/Graphics/TextUI.h"
#include "System/ResourceManager.h"

#include "Renderer/Renderer.h"
#include "Common/CastString.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	void TextUI::Draw(Renderer& renderer)
	{
		// AssetManagerがなければ何もしない
		if (!m_assetManager) return;

		// フォントを取得
		Font* font = m_assetManager->Get<Font>(m_fontHandle);

		// 出来なければ何もしない
		if (!font) return;

		RectTransform* transform = GetRectTransform();

		// ピボットのピクセル座標
		DirectX::SimpleMath::Vector2 pivotPixelPos = transform->GetPivotPixelPos();

		DirectX::SimpleMath::Vector2 pivot = transform->GetPivot();
		DirectX::SimpleMath::Vector2 disPivot = DirectX::SimpleMath::Vector2{ 1 - pivot.x, 1 - pivot.y };

		DirectX::SimpleMath::Vector2 size = transform->GetSize();
		DirectX::SimpleMath::Vector2 scale = transform->GetWorldScale();

		float defaultFontSize = font->GetFontSize();

		float fontScale = m_fontSize / defaultFontSize;

		// 描画位置を算出
		DirectX::SimpleMath::Vector2 drawPos =
			transform->GetLUPixelPos() +
			transform->GetRight() * size.x * scale.x * m_origin.x +
			transform->GetUp() * size.y * scale.y * m_origin.y;

		std::wstring s = CastString::Utf8ToWide(m_text);

		renderer.Draw().Text().Draw(
			font->Get(), s,
			drawPos, scale * fontScale, transform->GetWorldRotation(), m_origin, GetColor() * GetMulColor()
		);
	}
}	// namespace REngine
