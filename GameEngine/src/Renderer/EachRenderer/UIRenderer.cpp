//====================================================//
// ファイル名  : UIRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/09
//
// 概要        :  UI描画クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/EachRenderer/UIRenderer.h"
#include "System/WindowManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIRenderer::UIRenderer(DrawCommandContainer& container)
		: m_container{ container }
		, m_windowManager{ WindowManager::Instance() }
	{}

	void UIRenderer::DrawTriangle(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::SimpleMath::Vector2 p3, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		DirectX::VertexPositionColorTexture v1(m_windowManager.ScreenToPixel(p1), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v2(m_windowManager.ScreenToPixel(p2), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v3(m_windowManager.ScreenToPixel(p3), color, DirectX::SimpleMath::Vector2::Zero);

		if (auto* cmd = m_container.GetLatestPrimitiveCommand())
		{
			// 頂点を追加
			if (fillFlag)
			{
				cmd->triangles.emplace_back(v1, v2, v3);
			}
			else
			{
				cmd->lines.emplace_back(v1, v2);
				cmd->lines.emplace_back(v2, v3);
				cmd->lines.emplace_back(v3, v1);
			}	
		}
	}

	void UIRenderer::DrawRect(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::SimpleMath::Vector2 p3, DirectX::SimpleMath::Vector2 p4, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		DirectX::VertexPositionColorTexture v1(m_windowManager.ScreenToPixel(p1), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v2(m_windowManager.ScreenToPixel(p2), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v3(m_windowManager.ScreenToPixel(p3), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v4(m_windowManager.ScreenToPixel(p4), color, DirectX::SimpleMath::Vector2::Zero);

		if (auto* cmd = m_container.GetLatestPrimitiveCommand())
		{
			// 頂点を追加
			if (fillFlag)
			{
				cmd->triangles.emplace_back(v1, v2, v3);
				cmd->triangles.emplace_back(v1, v3, v4);
			}
			else
			{
				cmd->lines.emplace_back(v1, v2);
				cmd->lines.emplace_back(v2, v3);
				cmd->lines.emplace_back(v3, v4);
				cmd->lines.emplace_back(v4, v1);
			}
		}
	}

	void UIRenderer::DrawLine(DirectX::SimpleMath::Vector2 start, DirectX::SimpleMath::Vector2 end, DirectX::SimpleMath::Color color)
	{
		DirectX::VertexPositionColorTexture v1(m_windowManager.ScreenToPixel(start), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v2(m_windowManager.ScreenToPixel(end), color, DirectX::SimpleMath::Vector2::Zero);

		if (auto* cmd = m_container.GetLatestPrimitiveCommand())
		{
			// 頂点を追加
			cmd->lines.emplace_back(v1, v2);
		}
	}

	void UIRenderer::DrawCircle(DirectX::SimpleMath::Vector2 centerPos, float radius, uint16_t division, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		if (division < 3) return;

		// 角度の間隔を算出
		float step = 2.0f * PI_F / static_cast<float>(division);

		// 中心点
		DirectX::VertexPositionColorTexture c(m_windowManager.ScreenToPixel(centerPos), color, DirectX::SimpleMath::Vector2::Zero);

		// 最初の点
		DirectX::VertexPositionColorTexture prev(m_windowManager.ScreenToPixel(DirectX::SimpleMath::Vector2{ centerPos.x + radius, centerPos.y }), color, DirectX::SimpleMath::Vector2::Zero);

		// コマンドコンテナを取得
		auto* cmd = m_container.GetLatestPrimitiveCommand();
		if (!cmd)return;

		// 分割数分ループ
		for (int i = 0; i < division; ++i)
		{
			float theta = step * i;

			// 頂点情報を計算
			DirectX::VertexPositionColorTexture v(m_windowManager.ScreenToPixel(DirectX::SimpleMath::Vector2{ centerPos.x + cosf(theta) * radius, centerPos.y + sinf(theta) * radius }), color, DirectX::SimpleMath::Vector2::Zero);

			// 追加
			if (fillFlag) cmd->triangles.emplace_back(c, prev, v);

			else cmd->lines.emplace_back(prev, v);

			// 更新
			prev = v;
		}
	}

	void UIRenderer::DrawBox(DirectX::SimpleMath::Vector2 min, DirectX::SimpleMath::Vector2 max, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		DirectX::SimpleMath::Vector2 p1{ min.x, min.y };
		DirectX::SimpleMath::Vector2 p2{ min.x, max.y };
		DirectX::SimpleMath::Vector2 p3{ max.x, max.y };
		DirectX::SimpleMath::Vector2 p4{ max.x, min.y };

		DirectX::VertexPositionColorTexture v1(m_windowManager.ScreenToPixel(p1), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v2(m_windowManager.ScreenToPixel(p2), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v3(m_windowManager.ScreenToPixel(p3), color, DirectX::SimpleMath::Vector2::Zero);
		DirectX::VertexPositionColorTexture v4(m_windowManager.ScreenToPixel(p4), color, DirectX::SimpleMath::Vector2::Zero);

		if (auto* cmd = m_container.GetLatestPrimitiveCommand())
		{
			// 頂点を追加
			if (fillFlag)
			{
				cmd->triangles.emplace_back(v1, v2, v3);
				cmd->triangles.emplace_back(v1, v3, v4);
			}
			else
			{
				cmd->lines.emplace_back(v1, v2);
				cmd->lines.emplace_back(v2, v3);
				cmd->lines.emplace_back(v3, v4);
				cmd->lines.emplace_back(v4, v1);
			}
		}
	}

#pragma endregion
}	// namespace REngine
