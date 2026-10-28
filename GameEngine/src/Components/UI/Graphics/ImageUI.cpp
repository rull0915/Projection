/**
 * @file    ImageUI.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/23
 */

#include "pch.h"
#include "Components/UI/Graphics/ImageUI.h"

#include "Renderer/Renderer.h"
#include "Assets/Managers/AssetManager.h"
#include "Components/UI/RectTransform/RectTransform.h"

namespace RHEngine
{
	void ImageUI::Draw(Renderer& renderer)
	{
		// AssetManagerがなければ何もしない
		if (!m_pAssetManager) return;

		// ハンドルからTextureを取得
		Texture* texture = m_pAssetManager->Get<Texture>(m_textureHandle);

		// なければ何もしない
		if (!texture) return;

		RectTransform* pRectTransform = GetRectTransform();

		// ピボットのピクセル座標
		DirectX::SimpleMath::Vector2 pivotPixelPos = pRectTransform->GetPivotPixelPos();

		DirectX::SimpleMath::Vector2 pivot = pRectTransform->GetPivot();
		DirectX::SimpleMath::Vector2 disPivot = DirectX::SimpleMath::Vector2{ 1 - pivot.x, 1 - pivot.y };

		DirectX::SimpleMath::Vector2 size = pRectTransform->GetSize();

		// 有効なハンドルならセットする
		if (m_materialHandle.IsValid())
		{
			renderer.SetMaterial(m_materialHandle);
		}

		renderer.Draw().Sprite().Draw(
			texture->Get(),
			pivotPixelPos - size * pivot,
			pivotPixelPos + size * disPivot,
			pRectTransform->GetWorldScale(), 
			pRectTransform->GetWorldRotation(),
			pivot,
			GetColor() * GetMulColor()
		);

		// マテリアルのリセット
		renderer.SetMaterial(ERROR_HANDLE<MaterialAsset>);
	}
}	// namespace RHEngine
