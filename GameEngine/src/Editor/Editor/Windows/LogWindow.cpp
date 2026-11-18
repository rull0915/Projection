/**
 * @file    LogWindow.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/11/17
 */

#include "pch.h"
#include "LogWindow.h"

#ifdef ENGINE_GUI

#include "ThirdParty/imgui/imgui.h"
#include "Common/LogSystem/Logger.h"

namespace RHEngine
{
	LogWindow::LogWindow()
		: m_pSink{ nullptr }
		, m_visible{}
		, m_levelEnabled{}
		, m_autoScroll{ true }
	{
		// シンクを取得
		m_pSink = Logger::Instance().AddSink<ImGuiLogSink>();

		// 初期状態では全レベルを表示
		for (int i = 0; i < static_cast<int>(LogLevel::Count); i++)
		{
			m_levelEnabled[i] = true;
		}
	}

	void LogWindow::Draw()
	{
		if (!ImGui::Begin("Log")) { ImGui::End(); return; }

		// ---- ツールバー ----
		if (ImGui::Button("Clear")) m_pSink->Clear();
		ImGui::SameLine();
		ImGui::Checkbox("Auto-scroll", &m_autoScroll);
		ImGui::SameLine();

		static ImGuiTextFilter filter{};
		filter.Draw("Filter", 180.0f);

		// レベル別トグル
		for (int i = 0; i < 6; ++i)
		{
			DirectX::SimpleMath::Color col = LevelColor(static_cast<LogLevel>(i));
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, { col.x, col.y, col.z, col.w });
			ImGui::Checkbox(ToString(static_cast<LogLevel>(i)), &m_levelEnabled[i]);
			ImGui::PopStyleColor();
		}

		ImGui::Separator();

		ImGui::BeginChild("Logs");

		// シンク経由でエントリを取得する
		if (!m_pSink) return;

		m_pSink->Access([&](const std::deque<LogEntry>& entries)
			{
				// フィルタ通過した index を集める
				m_visible.clear();

				// 全エントリをチェック
				for (size_t i = 0; i < entries.size(); ++i)
				{
					// エントリを取得
					const auto& e = entries[i];

					// 表示チェック
					if (!m_levelEnabled[static_cast<int>(e.level)]) continue;

					// テキストフィルターチェック
					if (filter.IsActive() && !filter.PassFilter(e.message.c_str())) continue;

					// 全チェックを通過したら表示リストへ追加
					m_visible.push_back(i);
				}

				// 表示領域だけ描画
				ImGuiListClipper clipper;

				clipper.Begin((int)m_visible.size());

				while (clipper.Step())
				{
					// 表示対象の数分ループ
					for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
					{
						// エントリを取得
						const auto& e = entries[m_visible[row]];

						DirectX::SimpleMath::Color col = LevelColor(static_cast<LogLevel>(e.level));
						ImGui::PushStyleColor(ImGuiCol_Text, { col.x, col.y, col.z, col.w });

						ImGui::Text("%s f%llu [%s] %s",
							e.time.c_str(), e.frame, ToString(e.level), e.message.c_str());

						ImGui::PopStyleColor();

						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("%s:%u (thread %u)", e.file.c_str(), e.line, e.threadId);
					}
				}
				clipper.End();
			});

		// 最下部にいるときだけ自動追従
		if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f)
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();

		ImGui::End();
	}
}

#endif // ENGINE_GUI
