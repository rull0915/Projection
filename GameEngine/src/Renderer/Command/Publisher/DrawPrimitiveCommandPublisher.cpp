/**
 * @file    DrawPrimitiveCommandPublisher.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/30
 */

#include "pch.h"
#include "Renderer/Command/Publisher/DrawPrimitiveCommandPublisher.h"

namespace RHEngine
{
	DrawPrimitiveCommandPublisher::DrawPrimitiveCommandPublisher(DrawCommandContainer& container)
		: m_container{ container }
	{}

	void DrawPrimitiveCommandPublisher::DrawTriangle(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& p3, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		VertexPositionColorTexture v1(p1, color, DirectX::SimpleMath::Vector2::Zero);
		VertexPositionColorTexture v2(p2, color, DirectX::SimpleMath::Vector2::Zero);
		VertexPositionColorTexture v3(p3, color, DirectX::SimpleMath::Vector2::Zero);

		// 頂点を追加
		if (fillFlag)
		{
			m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, { v1, v2, v3 });
		}
		else
		{
			m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { v1, v2, v2, v3, v3, v1 });
		}
	}

	void DrawPrimitiveCommandPublisher::DrawTriangle(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector2& uv1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector2& uv2, const DirectX::SimpleMath::Vector3& p3, const DirectX::SimpleMath::Vector2& uv3, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		VertexPositionColorTexture v1(p1, color, uv1);
		VertexPositionColorTexture v2(p2, color, uv2);
		VertexPositionColorTexture v3(p3, color, uv3);

		// 頂点を追加
		if (fillFlag)
		{
			m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, { v1, v2, v3 });
		}
		else
		{
			m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { v1, v2, v2, v3, v3, v1 });
		}
	}

	void DrawPrimitiveCommandPublisher::DrawRect(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& p3, const DirectX::SimpleMath::Vector3& p4, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		VertexPositionColorTexture v1(p1, color, DirectX::SimpleMath::Vector2::Zero);
		VertexPositionColorTexture v2(p2, color, DirectX::SimpleMath::Vector2::Zero);
		VertexPositionColorTexture v3(p3, color, DirectX::SimpleMath::Vector2::Zero);
		VertexPositionColorTexture v4(p4, color, DirectX::SimpleMath::Vector2::Zero);

		// 頂点を追加
		if (fillFlag)
		{
			m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, { v1, v2, v3, v1, v3, v4 });
		}
		else
		{
			m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { v1, v2, v2, v3, v3, v4, v4, v1 });
		}
	}

	void DrawPrimitiveCommandPublisher::DrawRect(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector2& uv1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector2& uv2, const DirectX::SimpleMath::Vector3& p3, const DirectX::SimpleMath::Vector2& uv3, const DirectX::SimpleMath::Vector3& p4, const DirectX::SimpleMath::Vector2& uv4, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		VertexPositionColorTexture v1(p1, color, uv1);
		VertexPositionColorTexture v2(p2, color, uv2);
		VertexPositionColorTexture v3(p3, color, uv3);
		VertexPositionColorTexture v4(p4, color, uv4);

		// 頂点を追加
		if (fillFlag)
		{
			m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, { v1, v2, v3, v1, v3, v4 });
		}
		else
		{
			m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { v1, v2, v2, v3, v3, v4, v4, v1 });
		}
	}

	void DrawPrimitiveCommandPublisher::DrawLine(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, DirectX::SimpleMath::Color color)
	{
		VertexPositionColorTexture v1(p1, color, DirectX::SimpleMath::Vector2::Zero);
		VertexPositionColorTexture v2(p2, color, DirectX::SimpleMath::Vector2::Zero);

		m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { v1, v2 });
	}

	void DrawPrimitiveCommandPublisher::DrawCircle(const DirectX::SimpleMath::Vector3& centerPos, const DirectX::SimpleMath::Vector3& normal, float radius, uint16_t division, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		if (division < 3) return;

		// --- 基底ベクトルの計算 ---
		DirectX::SimpleMath::Vector3 up = (std::abs(normal.y) > 0.9f) ? DirectX::SimpleMath::Vector3::UnitX : DirectX::SimpleMath::Vector3::UnitY;
		DirectX::SimpleMath::Vector3 vU = normal.Cross(up);
		vU.Normalize();
		DirectX::SimpleMath::Vector3 vV = normal.Cross(vU);
		vV.Normalize();

		// 角度の間隔を算出
		float step = 2.0f * PI_F / static_cast<float>(division);

		// 中心
		VertexPositionColorTexture c = VertexPositionColorTexture(centerPos, color, DirectX::SimpleMath::Vector2::Zero);

		// 最初の点
		VertexPositionColorTexture prev(centerPos + vU * radius, color, DirectX::SimpleMath::Vector2::Zero);

		// 分割数分ループ
		for (int i = 1; i <= division; ++i)
		{
			// 角度
			float theta = step * i;

			// 頂点情報を計算
			VertexPositionColorTexture v(centerPos + (vU * cosf(theta) + vV * sinf(theta)) * radius, color, DirectX::SimpleMath::Vector2::Zero);

			// 頂点を追加
			if (fillFlag) m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, { prev, v, c });

			else m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { prev, v });

			// 更新
			prev = v;
		}
	}

	void DrawPrimitiveCommandPublisher::DrawArc(const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& vStart, const DirectX::SimpleMath::Vector3& vEnd, uint16_t division, float radius, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		if (radius <= 0.0f || division <= 0) return;

		// 平面の法線を求める
		DirectX::SimpleMath::Vector3 normal = vStart.Cross(vEnd);

		// 法線が決まらなかった場合スキップ
		if (normal.LengthSquared() < 0.000001f) {
			return;
		}
		normal.Normalize();

		// 基底ベクトル U, V を作成
		// vStart を角度 0 とする
		DirectX::SimpleMath::Vector3 vU = vStart;
		// U と法線に垂直なベクトルを V とする
		DirectX::SimpleMath::Vector3 vV = normal.Cross(vU);
		vU.Normalize();
		vV.Normalize();

		// 終了角を求める
		float endRadian = std::atan2(vV.Dot(vEnd), vU.Dot(vEnd));

		// 描画
		float step = endRadian / static_cast<float>(division);

		// 中心
		VertexPositionColorTexture c(center, color, DirectX::SimpleMath::Vector2::Zero);

		// 最初の点
		VertexPositionColorTexture prev(center + vU * radius, color, DirectX::SimpleMath::Vector2::Zero);

		// コマンドコンテナを取得
		auto* cmd = m_container.GetLatestPrimitiveCommand(fillFlag ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		if (!cmd) return;

		for (int i = 1; i <= division; ++i)
		{
			float theta = step * i;

			// 円周上の座標を算出
			VertexPositionColorTexture v(center + (vU * cosf(theta) + vV * sinf(theta)) * radius, color, DirectX::SimpleMath::Vector2::Zero);

			// 頂点を追加
			if (fillFlag) m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, { prev, v, c });

			else m_container.AddPrimitiveVertices(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, { prev, v });

			// 更新
			prev = v;
		}
	}
}	// namespace RHEngine
