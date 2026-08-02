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
#include "Components/UI/Graphics/ImageUI.h"

#include "Renderer/Renderer.h"
#include "Assets/Managers/AssetManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="renderer"></param>
	void ImageUI::Draw(Renderer& renderer)
	{
		// AssetManagerがなければ何もしない
		if (!m_assetManager) return;

		// ハンドルからTextureを取得
		Texture* texture = m_assetManager->Get<Texture>(m_textureHandle);

		// なければ何もしない
		if (!texture) return;

		RectTransform* transform = GetRectTransform();

		// ピボットのピクセル座標
		DirectX::SimpleMath::Vector2 pivotPixelPos = transform->GetPivotPixelPos();

		DirectX::SimpleMath::Vector2 pivot = transform->GetPivot();
		DirectX::SimpleMath::Vector2 disPivot = DirectX::SimpleMath::Vector2{ 1 - pivot.x, 1 - pivot.y };

		DirectX::SimpleMath::Vector2 size = transform->GetSize();

		renderer.Draw().Sprite().Draw(
			texture->Get(),
			pivotPixelPos - size * pivot,
			pivotPixelPos + size * disPivot,
			transform->GetWorldScale(), 
			transform->GetWorldRotation(),
			pivot,
			GetColor() * GetMulColor()
		);
	}
}	// namespace REngine
