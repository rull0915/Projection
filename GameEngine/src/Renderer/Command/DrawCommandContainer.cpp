/**
 * @file    DrawCommandContainer.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/10/27
 */

#include "pch.h"
#include "Renderer/Command/DrawCommandContainer.h"

namespace RHEngine
{
	DrawPrimitiveCommand& DrawCommandContainer::AddPrimitive()
	{
		// もし末尾のコマンドがなければそのまま返す
		if (!m_drawPrimitiveCommands.empty() && m_drawPrimitiveCommands.back().vertices.empty())
		{
			return m_drawPrimitiveCommands.back();
		}

		// 追加する
		auto& cmd = m_drawPrimitiveCommands.emplace_back();
		cmd.vertices.reserve(DrawPrimitiveCommand::MAX_VERTEX_COUNT);

		return cmd;
	}

	DrawPrimitiveCommand* RHEngine::DrawCommandContainer::GetLatestPrimitiveCommand(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		// なければ新しく作る
		if (m_drawPrimitiveCommands.empty()) return &AddPrimitive();

		// 最新コマンドを取得
		auto& latest = m_drawPrimitiveCommands.back();

		// トポロジーが変わるなら
		if (latest.topology != topology)
		{
			// 追加
			auto& newCommand = AddPrimitive();

			// ワールドを引き継ぐ
			newCommand.world = latest.world;

			newCommand.topology = topology;

			return &newCommand;
		}

		return &latest;
	}

	void DrawCommandContainer::AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY topology, std::initializer_list<VertexPositionColorTexture> vertices)
	{
		// 最新コマンドを取得
		auto latest = GetLatestPrimitiveCommand(topology);

		// もし追加後最大頂点数を超える場合
		if (latest->vertices.size() + vertices.size() >= DrawPrimitiveCommand::MAX_VERTEX_COUNT)
		{
			// 新しいコマンドを作成する
			latest = &AddPrimitive();
		}

		// 頂点に追加する
		latest->vertices.insert(latest->vertices.end(), vertices);
	}
}	// namespace RHEngine
