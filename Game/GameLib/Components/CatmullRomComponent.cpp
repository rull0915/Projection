//====================================================//
// ファイル名  : CatmullRomComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/10
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "CatmullRomComponent.h"

#include "Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
CatmullRomComponent::CatmullRomComponent(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_isLoop{ false }
	, m_offset{}
	, m_p0{}
	, m_p1{}
	, m_p2{}
	, m_p3{}
{
	ADD_PROPERTY(m_isLoop);
	ADD_PROPERTY(m_offset);
	ADD_PROPERTY(m_p0);
	ADD_PROPERTY(m_p1);
	ADD_PROPERTY(m_p2);
	ADD_PROPERTY(m_p3);
}

void CatmullRomComponent::DebugRender(REngine::Renderer & renderer, const DirectX::SimpleMath::Color & color)
{
	// 描画
	const size_t count = 100;

	// 始点を開始点に
	DirectX::SimpleMath::Vector3 prev = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 now = m_p1 + m_offset;

	// 点
	DirectX::SimpleMath::Vector3 points[4] = { m_p0, m_p1, m_p2, m_p3 };
	
	for (size_t index = 0; index < (m_isLoop ? 4 : 1); ++index)
	{
		// ループ
		for (int i = 1; i < count; ++i)
		{
			// 前の点を取得
			prev = now;

			// 割合を算出
			float t = static_cast<float>(i) / count;

			// 現在の座標を取得
			now = DirectX::SimpleMath::Vector3::CatmullRom(
				points[(index + 0) % 4],
				points[(index + 1) % 4],
				points[(index + 2) % 4],
				points[(index + 3) % 4],
				t
			) + m_offset;

			// 描画
			renderer.Draw().Primitive().DrawLine(prev, now, color);
		}
	}
}

DirectX::SimpleMath::Vector3 CatmullRomComponent::GetValue(float ratio)
{
	// ループしない場合
	if (!m_isLoop)
	{
		// ratioを0-1にクランプする
		float newRatio = std::clamp(ratio, 0.0f, 1.0f);

		// 値を返す
		return DirectX::SimpleMath::Vector3::CatmullRom(m_p0, m_p1, m_p2, m_p3, newRatio) + m_offset;
	}
	// ループする場合
	else
	{
		// 割合の整数部分を取得
		int intRatio = static_cast<int>(ratio);

		// 割合の小数部分を取得
		float fltRatio = ratio - intRatio;

		// 点を配列化
		DirectX::SimpleMath::Vector3 points[4] =
			{ m_p0, m_p1, m_p2, m_p3 };

		// 現在の座標を取得
		return
			DirectX::SimpleMath::Vector3::CatmullRom(
				points[(intRatio + 0) % 4],
				points[(intRatio + 1) % 4],
				points[(intRatio + 2) % 4],
				points[(intRatio + 3) % 4],
				fltRatio) + m_offset;
	}
}

DirectX::SimpleMath::Vector3 CatmullRomComponent::GetTangent(float ratio)
{
	// デルタタイム
	static constexpr float dt = 0.001f;

	// 2点を計算
	DirectX::SimpleMath::Vector3 p0 = GetValue(ratio);
	DirectX::SimpleMath::Vector3 p1 = GetValue(ratio + dt);

	// 2点間のベクトルを取得
	DirectX::SimpleMath::Vector3 dir = p1 - p0;

	// 正規化
	dir.Normalize();

	// 返す
	return dir;
}
