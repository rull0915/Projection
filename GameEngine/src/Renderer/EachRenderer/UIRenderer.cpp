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
	{}

	void UIRenderer::DrawTriangle(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::SimpleMath::Vector2 p3, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		DirectX::VertexPositionColor v1(WindowManager::Instance().ScreenToPixel(p1), color);
		DirectX::VertexPositionColor v2(WindowManager::Instance().ScreenToPixel(p2), color);
		DirectX::VertexPositionColor v3(WindowManager::Instance().ScreenToPixel(p3), color);

		if (auto* cmd = m_container.GetLatestPrimitiveCommand())
		{
			// 頂点を追加
			if (fillFlag) cmd->triangles.insert(cmd->triangles.end(), { {v1, v2, v3} });

			else cmd->lines.insert(cmd->lines.end(), { { v1, v2 }, { v2, v3 }, { v3, v1 } });
		}
	}

	void UIRenderer::DrawRect(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::SimpleMath::Vector2 p3, DirectX::SimpleMath::Vector2 p4, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		DirectX::VertexPositionColor v1(WindowManager::Instance().ScreenToPixel(p1), color);
		DirectX::VertexPositionColor v2(WindowManager::Instance().ScreenToPixel(p2), color);
		DirectX::VertexPositionColor v3(WindowManager::Instance().ScreenToPixel(p3), color);
		DirectX::VertexPositionColor v4(WindowManager::Instance().ScreenToPixel(p4), color);

		if (auto* cmd = m_container.GetLatestPrimitiveCommand())
		{
			// 頂点を追加
			if (fillFlag) cmd->triangles.insert(cmd->triangles.end(), { {v1, v2, v3}, {v1, v3, v4} });

			else cmd->lines.insert(cmd->lines.end(), { { v1, v2 }, { v2, v3 }, { v3, v4 }, { v4, v2 } });
		}
	}

	void UIRenderer::DrawLine(DirectX::SimpleMath::Vector2 start, DirectX::SimpleMath::Vector2 end, DirectX::SimpleMath::Color color)
	{
		DirectX::VertexPositionColor v1(WindowManager::Instance().ScreenToPixel(start), color);
		DirectX::VertexPositionColor v2(WindowManager::Instance().ScreenToPixel(end), color);

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
		DirectX::VertexPositionColor c(WindowManager::Instance().ScreenToPixel(centerPos), color);

		// 最初の点
		DirectX::VertexPositionColor prev(WindowManager::Instance().ScreenToPixel(DirectX::SimpleMath::Vector2{ centerPos.x + radius, centerPos.y }), color);

		// コマンドコンテナを取得
		auto* cmd = m_container.GetLatestPrimitiveCommand();
		if (!cmd)return;

		// 分割数分ループ
		for (int i = 0; i < division; ++i)
		{
			float theta = step * i;

			// 頂点情報を計算
			DirectX::VertexPositionColor v(WindowManager::Instance().ScreenToPixel(DirectX::SimpleMath::Vector2{ centerPos.x + cosf(theta) * radius, centerPos.y + sinf(theta) * radius }), color);

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

		DirectX::VertexPositionColor v1(WindowManager::Instance().ScreenToPixel(p1), color);
		DirectX::VertexPositionColor v2(WindowManager::Instance().ScreenToPixel(p2), color);
		DirectX::VertexPositionColor v3(WindowManager::Instance().ScreenToPixel(p3), color);
		DirectX::VertexPositionColor v4(WindowManager::Instance().ScreenToPixel(p4), color);

		if (auto* cmd = m_container.GetLatestPrimitiveCommand())
		{
			// 頂点を追加
			if (fillFlag) cmd->triangles.insert(cmd->triangles.end(), { {v1, v2, v3}, {v1, v3, v4} });

			else cmd->lines.insert(cmd->lines.end(), { { v1, v2 }, { v2, v3 }, { v3, v4 }, { v4, v1 } });
		}
	}

#pragma endregion
}	// namespace REngine
