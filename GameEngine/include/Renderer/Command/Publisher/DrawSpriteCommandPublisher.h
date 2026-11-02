/**
 * @file    DrawSpriteCommandPublisher.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/29
 *
 * @brief   スプライト描画コマンドを発行する
 */

#pragma once

#include <Windows.h>

#include "Renderer/Command/DrawCommandContainer.h"
#include "Renderer/GraphicsSystem.h"

namespace RHEngine
{
	/**
	 * @brief スプライト描画コマンドを発行する
	 */
	class DrawSpriteCommandPublisher
	{
	private:

		/** @brief コマンドのコンテナ */
		DrawCommandContainer& m_commandContainer;

		/** @brief 描画システム */
		GraphicsSystem& m_graphicSystem;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param container コマンドコンテナ
		 * @param system 描画システム
		 */
		DrawSpriteCommandPublisher(DrawCommandContainer& container, GraphicsSystem& system);

		~DrawSpriteCommandPublisher() = default;

		/// @}

		/**
		 * @brief スプライト描画コマンドの発行
		 * @param texture テクスチャ
		 * @param pos 左上の位置
		 * @param scale スケール
		 * @param angle 回転角度
		 * @param color 乗算色
		 */
		void Draw(
			ID3D11ShaderResourceView* texture,
			DirectX::SimpleMath::Vector2 pos,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Color color
		);
		
		/**
		 * @brief スクリーン全体にテクスチャを描画するコマンド
		 * @param texture テクスチャ
		 * @param color 乗算色
		 */
		void DrawFullScreen(
			ID3D11ShaderResourceView* texture,
			DirectX::SimpleMath::Color color = { 1, 1, 1, 1 }
		);

		/**
		 * @brief 切り取る領域を指定する描画コマンド
		 * @param texture テクスチャ
		 * @param min 描画領域のmin
		 * @param max 描画領域のmax
		 * @param scale スケール
		 * @param angle 回転角度
		 * @param color 乗算色
		 */
		void Draw(
			ID3D11ShaderResourceView* texture,
			DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Color color
		);

		/**
		 * @brief 原点を指定する描画コマンド
		 * @param texture テクスチャ
		 * @param pos 位置 (原点がposに配置される)
		 * @param scale スケール (原点基準で拡縮される)
		 * @param angle 回転角度 (原点基準で回転する)
		 * @param origin 原点 (uv座標)
		 * @param color 乗算色
		 */
		void Draw(
			ID3D11ShaderResourceView* texture,
			DirectX::SimpleMath::Vector2 pos,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		);

		/**
		 * @brief 描画領域と原点を指定する描画コマンド
		 * @param texture テクスチャ
		 * @param min 描画領域のmin
		 * @param max 描画領域のmax
		 * @param scale スケール (原点基準で拡縮される)
		 * @param angle 回転角度 (原点基準で回転する)
		 * @param origin 原点 (uv座標)
		 * @param color 乗算色
		 */
		void Draw(
			ID3D11ShaderResourceView* texture,
			DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		);

		/**
		 * @brief 描画領域と原点を指定する描画コマンド
		 * @param texture テクスチャ
		 * @param pos 位置 (原点がposに配置される)
		 * @param srcRect 切り取る領域
		 * @param scale スケール (原点基準で拡縮される)
		 * @param angle 回転角度 (原点基準で回転する)
		 * @param origin 原点 (uv座標)
		 * @param color 乗算色
		 */
		void Draw(
			ID3D11ShaderResourceView* texture,
			DirectX::SimpleMath::Vector2 pos,
			RECT* srcRect,
			DirectX::SimpleMath::Vector2 scale,
			float angle,
			DirectX::SimpleMath::Vector2 origin,
			DirectX::SimpleMath::Color color
		);

		static DirectX::SimpleMath::Vector2 GetTextureSize(ID3D11ShaderResourceView* srv);
	};
}	// namespace RHEngine
