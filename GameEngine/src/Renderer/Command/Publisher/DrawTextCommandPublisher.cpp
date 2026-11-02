/**
 * @file    DrawTextCommandPublisher.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/01
 */

#include "pch.h"
#include "Renderer/Command/Publisher/DrawTextCommandPublisher.h"

namespace RHEngine
{
	DrawTextCommandPublisher::DrawTextCommandPublisher(DrawCommandContainer& container, GraphicsSystem& graphicSystem)
		: m_commandContainer{ container }
		, m_graphicSystem{ graphicSystem }
	{}

	void DrawTextCommandPublisher::Draw(DirectX::SpriteFont* font, const std::wstring& text, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Color color)
	{
		// コマンドの追加
		auto& command = m_commandContainer.AddText();

		command.spriteFont = font;
		command.text = text;
		command.pos = pos;
		command.scale = scale;
		command.angle = angle;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

	void DrawTextCommandPublisher::Draw(DirectX::SpriteFont* font, const std::wstring& text, DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Color color)
	{
		// テクスチャサイズを取得
		DirectX::SimpleMath::Vector2 textureSize = DrawTextCommandPublisher::GetTextureSize(font, text.c_str());
		
		// スケールを計算
		DirectX::SimpleMath::Vector2 finalScale = (max - min) / textureSize;

		finalScale *= scale;

		// コマンドの追加
		auto& command = m_commandContainer.AddText();

		command.spriteFont = font;
		command.text = text;
		command.pos = min;
		command.scale = finalScale;
		command.angle = angle;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

	void DrawTextCommandPublisher::Draw(DirectX::SpriteFont* font, const std::wstring& text, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Vector2 origin, DirectX::SimpleMath::Color color)
	{
		// テクスチャサイズを取得
		DirectX::SimpleMath::Vector2 texSize = GetTextureSize(font, text.c_str());

		// 原点を算出
		DirectX::SimpleMath::Vector2 finalOrigin = origin * texSize;

		// コマンドの追加
		auto& command = m_commandContainer.AddText();

		command.spriteFont = font;
		command.text = text;
		command.pos = pos;
		command.scale = scale;
		command.angle = angle;
		command.origin = finalOrigin;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

	void DrawTextCommandPublisher::Draw(DirectX::SpriteFont* font, const std::wstring& text, DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Vector2 origin, DirectX::SimpleMath::Color color)
	{
		// テクスチャサイズを取得
		DirectX::SimpleMath::Vector2 texSize = GetTextureSize(font, text.c_str());

		// Originを考慮して中心を計算
		DirectX::SimpleMath::Vector2 center = min + (max - min) * origin;

		// 原点を算出
		DirectX::SimpleMath::Vector2 finalOrigin = origin * texSize;

		// スケールの計算
		DirectX::SimpleMath::Vector2 finalScale = (max - min) / texSize;

		// 指定の拡大率を乗算する
		finalScale *= scale;

		// コマンドの追加
		auto& command = m_commandContainer.AddText();

		command.spriteFont = font;
		command.text = text;
		command.pos = center;
		command.scale = finalScale;
		command.angle = angle;
		command.origin = finalOrigin;
		command.color = color;
		command.material = m_graphicSystem.GetMaterial();
	}

}	// namespace RHEngine
