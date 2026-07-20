//====================================================//
// ファイル名  : TextRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/01
//
// 概要        :  テキストの描画を担当する関数
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/EachRenderer/TextRenderer.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextRenderer::TextRenderer(DrawCommandContainer& container)
		: m_container{ container }
	{}

	/// <summary>
	/// デストラクタ
	/// </summary>
	TextRenderer::~TextRenderer()
	{}

	void TextRenderer::Draw(DirectX::SpriteFont* font, const std::wstring& text, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Color color)
	{
		// コマンドの追加
		auto& command = m_container.AddText();

		command.spriteFont = font;
		command.text = text;
		command.pos = pos;
		command.scale = scale;
		command.angle = angle;
		command.color = color;
	}

	void TextRenderer::Draw(DirectX::SpriteFont* font, const std::wstring& text, DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Color color)
	{
		// テクスチャサイズを取得
		DirectX::SimpleMath::Vector2 textureSize = TextRenderer::GetTextureSize(font, text.c_str());
		
		// スケールを計算
		DirectX::SimpleMath::Vector2 finalScale = (max - min) / textureSize;

		finalScale *= scale;

		// コマンドの追加
		auto& command = m_container.AddText();

		command.spriteFont = font;
		command.text = text;
		command.pos = min;
		command.scale = finalScale;
		command.angle = angle;
		command.color = color;
	}

	void TextRenderer::Draw(DirectX::SpriteFont* font, const std::wstring& text, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Vector2 origin, DirectX::SimpleMath::Color color)
	{
		// テクスチャサイズを取得
		DirectX::SimpleMath::Vector2 texSize = GetTextureSize(font, text.c_str());

		// 原点を算出
		DirectX::SimpleMath::Vector2 finalOrigin = origin * texSize;

		// コマンドの追加
		auto& command = m_container.AddText();

		command.spriteFont = font;
		command.text = text;
		command.pos = pos;
		command.scale = scale;
		command.angle = angle;
		command.origin = finalOrigin;
		command.color = color;
	}

	void TextRenderer::Draw(DirectX::SpriteFont* font, const std::wstring& text, DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, DirectX::SimpleMath::Vector2 scale, float angle, DirectX::SimpleMath::Vector2 origin, DirectX::SimpleMath::Color color)
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
		auto& command = m_container.AddText();

		command.spriteFont = font;
		command.text = text;
		command.pos = center;
		command.scale = finalScale;
		command.angle = angle;
		command.origin = finalOrigin;
		command.color = color;
	}

}	// namespace REngine
