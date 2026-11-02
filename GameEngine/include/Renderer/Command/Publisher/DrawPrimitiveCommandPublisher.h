/**
 * @file    DrawPrimitiveCommandPublisher.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/29
 *
 * @brief   プリミティブの描画のコマンドを発行するクラス
 */

#pragma once

#include "Renderer/Command/DrawCommandContainer.h"

namespace RHEngine
{
	/**
	 * @brief プリミティブ描画コマンドを発行する
	 */
	class DrawPrimitiveCommandPublisher
	{
	private:

		/** @brief コマンドコンテナ */
		DrawCommandContainer& m_container;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param container コマンドコンテナ
		 */
		DrawPrimitiveCommandPublisher(DrawCommandContainer& container);

		~DrawPrimitiveCommandPublisher() = default;

		/// @}

	public:

		/**
		 * @brief 三角形描画コマンドの発行
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawTriangle	
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector3& p2,
			const DirectX::SimpleMath::Vector3& p3,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		/**
		 * @brief 三角形描画コマンドの発行
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawTriangle	
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector2& uv1,
			const DirectX::SimpleMath::Vector3& p2,
			const DirectX::SimpleMath::Vector2& uv2,
			const DirectX::SimpleMath::Vector3& p3,
			const DirectX::SimpleMath::Vector2& uv3,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		/**
		 * @brief 四角形描画コマンドの発行
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawRect
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector3& p2,
			const DirectX::SimpleMath::Vector3& p3,
			const DirectX::SimpleMath::Vector3& p4,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		/**
		 * @brief 四角形描画コマンドの発行
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawRect
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector2& uv1,
			const DirectX::SimpleMath::Vector3& p2,
			const DirectX::SimpleMath::Vector2& uv2,
			const DirectX::SimpleMath::Vector3& p3,
			const DirectX::SimpleMath::Vector2& uv3,
			const DirectX::SimpleMath::Vector3& p4,
			const DirectX::SimpleMath::Vector2& uv4,
			DirectX::SimpleMath::Color color, bool fillFlag
		);

		/**
		 * @brief 線描画コマンドの発行
		 * @param color 描画色
		 */
		void DrawLine
		(
			const DirectX::SimpleMath::Vector3& p1,
			const DirectX::SimpleMath::Vector3& p2,
			DirectX::SimpleMath::Color color
		);

		/**
		 * @brief 円描画コマンドの発行
		 * @param centerPos 中心座標
		 * @param normal 描画する平面の法線ベクトル
		 * @param radius 半径
		 * @param division 分割数
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawCircle
		(
			const DirectX::SimpleMath::Vector3& centerPos,
			const DirectX::SimpleMath::Vector3& normal,
			float radius,
			uint16_t division,
			DirectX::SimpleMath::Color color,
			bool fillFlag
		);

		/**
		 * @brief 弧を描画するコマンドの発行
		 * @param center 中心座標
		 * @param vStart 開始のベクトル (正規化前提)
		 * @param vEnd 終了のベクトル (正規化前提)
		 * @param segments 分割数
		 * @param radius 半径
		 * @param color 描画色
		 * @param fillFlag 塗りつぶすかどうか
		 */
		void DrawArc(
			const DirectX::SimpleMath::Vector3& center,
			const DirectX::SimpleMath::Vector3& vStart,
			const DirectX::SimpleMath::Vector3& vEnd,
			uint16_t segments,
			float radius,
			DirectX::SimpleMath::Color color,
			bool fillFlag
		);
	};
}	// namespace RHEngine
