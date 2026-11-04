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
		cmd.vertices.reserve(1024);

		return cmd;
	}

	DrawPrimitiveCommand* RHEngine::DrawCommandContainer::GetLatestPrimitiveCommand(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		// なければnullをかえす
		if (m_drawPrimitiveCommands.empty()) return nullptr;

		// どちらかの配列が最大値を超えるなら
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
}	// namespace RHEngine
