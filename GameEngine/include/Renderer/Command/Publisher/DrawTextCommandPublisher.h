/**
 * @file    DrawTextCommandPublisher.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/29
 *
 * @brief   文字描画コマンドを発行する
 */

#pragma once

#include <SimpleMath.h>
#include <DirectXMath.h>
#include <DirectXMathVector.inl>

#include "Renderer/Command/DrawCommandContainer.h"
#include "Renderer/GraphicsSystem.h"

namespace RHEngine
{
	/**
	 * @brief 文字描画コマンドを発行する
	 */
	class DrawTextCommandPublisher
	{
	private:

		/** @brief コマンドのコンテナ */
		DrawCommandContainer& m_commandContainer;

		/** @brief 描画システム */
		GraphicsSystem& m_graphicSystem;

	public:

		/// @name ライフサイクル
		/// @{

		DrawTextCommandPublisher(DrawCommandContainer& container, GraphicsSystem& graphicSystem);

		~DrawTextCommandPublisher() = default;

		/// @}

		/**
		 * @brief 文字列描画コマンド
		 * @param font フォント
		 * @param text 文字列
		 * @param pos 描画位置
		 * @param scale スケール
		 * @param angle 回転角度
		 * @param color 乗算色
		 */
		void Draw(
			DirectX::SpriteFont* font,
			const std::wstring& text,
			DirectX::SimpleMath::Vector2 pos,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Color color
		);

		/**
		 * @brief 描画領域指定の文字列描画コマンド
		 * @param font フォント
		 * @param text 文字列
		 * @param min 描画領域のmin
		 * @param max 描画領域のmax
		 * @param scale スケール
		 * @param angle 回転角度
		 * @param color 乗算色
		 */
		void Draw(
			DirectX::SpriteFont* font,
			const std::wstring& text,
			DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Color color
		);

		/**
		 * @brief 原点指定の文字列描画コマンド
		 * @param font フォント
		 * @param text 文字列
		 * @param pos 描画位置 (原点がposに配置される)
		 * @param scale スケール (原点基準で拡縮する)
		 * @param angle 回転角度 (原点基準で回転する)
		 * @param origin 原点 (uv座標)
		 * @param color 乗算色
		 */
		void Draw(
			DirectX::SpriteFont* font,
			const std::wstring& text,
			DirectX::SimpleMath::Vector2 pos,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		);

		/**
		 * @brief 描画領域と原点を指定する文字列描画コマンド
		 * @param font フォント
		 * @param text 文字列
		 * @param min 描画領域のmin
		 * @param max 描画領域のmax
		 * @param scale スケール (原点基準で拡縮)
		 * @param angle 回転角度 (原点基準で回転)
		 * @param origin 原点 (uv座標)
		 * @param color 乗算色
		 */
		void Draw(
			DirectX::SpriteFont* font,
			const std::wstring& text,
			DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		);

		static DirectX::SimpleMath::Vector2 GetTextureSize(DirectX::SpriteFont* font, const wchar_t* text)
		{
			DirectX::XMVECTOR result = font->MeasureString(text);

			return { DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result) };
		}
	};
}	// namespace RHEngine
