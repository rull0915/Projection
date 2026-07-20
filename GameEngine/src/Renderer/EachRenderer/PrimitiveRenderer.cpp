//====================================================//
// ファイル名  : PrimitiveRenderer.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/04/30
//
// 概要        :  プリミティブ描画クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Renderer/EachRenderer/PrimitiveRenderer.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PrimitiveRenderer::PrimitiveRenderer(DrawCommandContainer& container)
		: m_container{ container }
	{}

	/// <summary>
	/// 三角形描画関数
	/// </summary>
	/// <param name="p1">頂点座標1</param>
	/// <param name="p2">頂点座標2</param>
	/// <param name="p3">頂点座標3</param>
	/// <param name="color">色</param>
	void PrimitiveRenderer::DrawTriangle(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& p3, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		DirectX::VertexPositionColor v1(p1, color);
		DirectX::VertexPositionColor v2(p2, color);
		DirectX::VertexPositionColor v3(p3, color);

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

	/// <summary>
	/// 矩形描画関数
	/// </summary>
	/// <param name="p1">頂点座標1</param>
	/// <param name="p2">頂点座標2</param>
	/// <param name="p3">頂点座標3</param>
	/// <param name="p4">頂点座標4</param>
	/// <param name="color">色</param>
	void PrimitiveRenderer::DrawRect(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& p3, const DirectX::SimpleMath::Vector3& p4, DirectX::SimpleMath::Color color, bool fillFlag)
	{
		DirectX::VertexPositionColor v1(p1, color);
		DirectX::VertexPositionColor v2(p2, color);
		DirectX::VertexPositionColor v3(p3, color);
		DirectX::VertexPositionColor v4(p4, color);

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

	void PrimitiveRenderer::DrawLine(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, DirectX::SimpleMath::Color color)
	{
		DirectX::VertexPositionColor v1(p1, color);
		DirectX::VertexPositionColor v2(p2, color);

		if (auto* cmd = m_container.GetLatestPrimitiveCommand())
		{
			// 頂点を追加
			cmd->lines.emplace_back(v1, v2);
		}
	}

	/// <summary>
	/// 円描画関数
	/// </summary>
	/// <param name="centerPos">中心座標</param>
	/// <param name="normal">描く面の法線</param>
	/// <param name="radius">半径</param>
	/// <param name="division">分割数</param>
	/// <param name="color">色</param>
	void PrimitiveRenderer::DrawCircle(const DirectX::SimpleMath::Vector3& centerPos, const DirectX::SimpleMath::Vector3& normal, float radius, uint16_t division, DirectX::SimpleMath::Color color, bool fillFlag)
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
		DirectX::VertexPositionColor c = DirectX::VertexPositionColor(centerPos, color);

		// 最初の点
		DirectX::VertexPositionColor prev(centerPos + vU * radius, color);

		// コマンドコンテナを取得
		auto* cmd = m_container.GetLatestPrimitiveCommand();
		if (!cmd) return;

		// 分割数分ループ
		for (int i = 1; i <= division; ++i)
		{
			// 角度
			float theta = step * i;

			// 頂点情報を計算
			DirectX::VertexPositionColor v(centerPos + (vU * cosf(theta) + vV * sinf(theta)) * radius, color);

			// 頂点を追加
			if (fillFlag) cmd->triangles.emplace_back(prev, v, c);

			else cmd->lines.emplace_back(prev, v);

			// 更新
			prev = v;
		}
	}

	/// <summary>
	/// 弧の描画関数
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="vStart">弧の開始位置</param>
	/// <param name="vEnd">弧の終了位置</param>
	/// <param name="division">分割数</param>
	/// <param name="radius">半径</param>
	/// <param name="color">色</param>
	void PrimitiveRenderer::DrawArc(const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& vStart, const DirectX::SimpleMath::Vector3& vEnd, uint16_t division, float radius, DirectX::SimpleMath::Color color, bool fillFlag)
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
		DirectX::VertexPositionColor c(center, color);

		// 最初の点
		DirectX::VertexPositionColor prev(center + vU * radius, color);

		// コマンドコンテナを取得
		auto* cmd = m_container.GetLatestPrimitiveCommand();
		if (!cmd) return;

		for (int i = 1; i <= division; ++i)
		{
			float theta = step * i;

			// 円周上の座標を算出
			DirectX::VertexPositionColor v(center + (vU * cosf(theta) + vV * sinf(theta)) * radius, color);

			// 頂点を追加
			if (fillFlag) cmd->triangles.emplace_back(prev, v, c);

			else cmd->lines.emplace_back(prev, v);

			// 更新
			prev = v;
		}
	}
}	// namespace REngine
