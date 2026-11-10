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
#include <d3d11.h>
#include <initializer_list>

#include "DrawCommand.h"
#include "Assets/Types/Vertex/VertexTypes.h"

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

		/** @brief スカイボックス描画コマンドリスト */
		std::vector<DrawSkyboxCommand> m_drawSkyboxCommands;

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
			m_drawSkyboxCommands.clear();
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

		/** @brief スカイボックス描画コマンドの追加 */
		DrawSkyboxCommand& AddSkybox() { return m_drawSkyboxCommands.emplace_back(); }

		/// @}

		/// @name コマンドの取得
		/// @{

		/** @brief  最新のプリミティブ描画コマンドを取得する関数 */
		DrawPrimitiveCommand* GetLatestPrimitiveCommand(D3D11_PRIMITIVE_TOPOLOGY topology);

		/** @brief  プリミティブ描画コマンドリスト */
		const std::vector<DrawPrimitiveCommand>& GetDrawPrimitiveCommands() const { return m_drawPrimitiveCommands; };

		/** @brief  モデル描画コマンドリスト */
		const std::vector<DrawMeshCommand>& GetDrawMeshCommands() const { return m_drawMeshCommands; }

		/** @brief  UI描画コマンドリスト */
		const std::vector<DrawUICommand>& GetDrawUICommands() const { return m_drawUICommands; }

		/** @brief  スカイボックス描画コマンドリスト */
		const std::vector<DrawSkyboxCommand>& GetDrawSkyboxCommands() const { return m_drawSkyboxCommands; }

		/// @}

		/**
		 * @brief プリミティブコマンドへ頂点を追加する
		 * @param vertices 追加対象の頂点リスト
		 */
		void AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY topology, std::initializer_list<VertexPositionColorTexture> vertices);
	};
}	// namespace RHEngine
