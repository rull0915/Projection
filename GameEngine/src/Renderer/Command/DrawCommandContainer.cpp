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
		if (!m_drawPrimitiveCommands.empty() && (m_drawPrimitiveCommands.back().lines.empty() && m_drawPrimitiveCommands.back().triangles.empty()))
		{
			return m_drawPrimitiveCommands.back();
		}

		// 追加する
		auto& cmd = m_drawPrimitiveCommands.emplace_back();
		cmd.lines.reserve(1024);
		cmd.triangles.reserve(512);

		return cmd;
	}

	DrawPrimitiveCommand* RHEngine::DrawCommandContainer::GetLatestPrimitiveCommand()
	{
		// なければnullをかえす
		if (m_drawPrimitiveCommands.empty()) return nullptr;

		// どちらかの配列が最大値を超えるなら
		auto& latest = m_drawPrimitiveCommands.back();
		if (latest.lines.size() > 750 || latest.triangles.size() > 500)
		{
			// 追加
			auto& newCommand = AddPrimitive();

			// ワールドを引き継ぐ
			newCommand.world = latest.world;

			return &newCommand;
		}

		return &latest;
	}
}	// namespace RHEngine
