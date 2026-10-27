/**
 * @file    DrawCommandContainer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/07/18
 *
 * @brief   描画コマンドをため込んでおくクラス
 */

#pragma once

#include <vector>
#include <variant>

#include "DrawCommand.h"

namespace RHEngine
{
	/**
	 * @brief 描画コマンドをため込む
	 */
	class DrawCommandContainer
	{
	public:
		using DrawUICommand = std::variant<DrawSpriteCommand, DrawTextCommand>;

	private:

		/** @brief プリミティブ描画コマンドリスト */
		std::vector<DrawPrimitiveCommand> m_drawPrimitiveCommands;

		/** @brief メッシュ描画コマンドリスト */
		std::vector<DrawMeshCommand> m_drawMeshCommands;

		/** @brief UI描画コマンドリスト */
		std::vector<DrawUICommand> m_drawUICommands;

	public:

		/// @name ライフサイクル
		/// @{

		DrawCommandContainer() = default;

		~DrawCommandContainer() = default;

		/// @}

		/**
		 * @brief 配列のクリア
		 */
		void Clear()
		{
			m_drawPrimitiveCommands.clear();
			m_drawMeshCommands.clear();
			m_drawUICommands.clear();
		};

		/// @name コマンドの追加
		/// @{

		/** @brief プリミティブ描画コマンドを追加する */
		DrawPrimitiveCommand& AddPrimitive();

		/** @brief メッシュ描画コマンドの追加 */
		DrawMeshCommand& AddModel() { return m_drawMeshCommands.emplace_back(); }

		/** @brief スプライト描画コマンドの追加 */
		DrawSpriteCommand& AddSprite() { return std::get<DrawSpriteCommand>(m_drawUICommands.emplace_back(DrawSpriteCommand())); }

		/** @brief テキスト描画コマンドの追加 */
		DrawTextCommand& AddText() { return std::get<DrawTextCommand>(m_drawUICommands.emplace_back(DrawTextCommand())); }

		/// @}

		/// @name コマンドの取得
		/// @{

		/** @brief  最新のプリミティブ描画コマンドを取得する関数 */
		DrawPrimitiveCommand* GetLatestPrimitiveCommand();

		/** @brief  プリミティブ描画コマンドリスト */
		const std::vector<DrawPrimitiveCommand>& GetDrawPrimitiveCommands() const { return m_drawPrimitiveCommands; };

		/** @brief  モデル描画コマンドリスト */
		const std::vector<DrawMeshCommand>& GetDrawMeshCommands() const { return m_drawMeshCommands; }

		/** @brief  UI描画コマンドリスト */
		const std::vector<DrawUICommand>& GetDrawUICommands() const { return m_drawUICommands; }

		/// @}
	};
}	// namespace RHEngine
