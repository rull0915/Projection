/**
 * @file    DrawUICommandPublisher.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/09
 *
 * @brief   UIの描画コマンドを発行する
 */

#pragma once

#include "Renderer/Command/DrawCommandContainer.h"

namespace RHEngine
{
	class WindowManager;

	/**
	 * @brief UIの描画コマンドを発行する
	 */
	class DrawUICommandPublisher
	{
	private:

		/** @brief コマンドのコンテナ */
		DrawCommandContainer& m_commandContainer;

		/** @brief ウィンドウマネージャー */
		WindowManager& m_windowManager;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param container コマンドコンテナ
		 */
		DrawUICommandPublisher(DrawCommandContainer& container);

		~DrawUICommandPublisher() = default;

		/// @}

	public:

		/**
		 * @brief 三角形描画コマンド
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawTriangle	// 短縮版
		(
			DirectX::SimpleMath::Vector2 p1,
			DirectX::SimpleMath::Vector2 p2,
			DirectX::SimpleMath::Vector2 p3,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		/**
		 * @brief 四角形描画コマンド
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawRect
		(
			DirectX::SimpleMath::Vector2 p1,
			DirectX::SimpleMath::Vector2 p2,
			DirectX::SimpleMath::Vector2 p3,
			DirectX::SimpleMath::Vector2 p4,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		/**
		 * @brief 線描画コマンド
		 * @param color 描画色
		 */
		void DrawLine
		(
			DirectX::SimpleMath::Vector2 start,
			DirectX::SimpleMath::Vector2 end,
			DirectX::SimpleMath::Color color
		);

		/**
		 * @brief 円描画
		 * @param centerPos 中心座標
		 * @param radius 半径
		 * @param division 分割数
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawCircle
		(
			DirectX::SimpleMath::Vector2 centerPos,
			float radius,
			uint16_t division,
			DirectX::SimpleMath::Color color,
			bool fillFlag
		);

		/**
		 * @brief 長方形描画コマンド
		 * @param min 左上
		 * @param max 右下
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawBox
		(
			DirectX::SimpleMath::Vector2 min,
			DirectX::SimpleMath::Vector2 max,
			DirectX::SimpleMath::Color color,
			bool fillFlag
		);
	};
}	// namespace RHEngine
