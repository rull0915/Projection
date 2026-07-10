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
CatmullRomComponent::CatmullRomComponent(IComponentOwner* own)
	: WorldComponentBase(own)
{
	ADD_PROPERTY(m_start);
	ADD_PROPERTY(m_end);
	ADD_PROPERTY(m_startControl);
	ADD_PROPERTY(m_endControl);
}

// 生成直後に一度呼ばれます
void CatmullRomComponent::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void CatmullRomComponent::Start()
{}

// 毎フレーム呼ばれます
void CatmullRomComponent::Update(const GameTimer & gameTimer)
{}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void CatmullRomComponent::LateUpdate(const GameTimer & gameTimer)
{}

void CatmullRomComponent::DebugRender(Renderer & renderer, const DirectX::SimpleMath::Color & color)
{
	// 描画
	const size_t count = 100;

	// 始点を開始点に
	DirectX::SimpleMath::Vector3 prev = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 now = m_start;

	// ループ
	for (int i = 1; i < count; ++i)
	{
		// 前の点を取得
		prev = now;

		// 割合を算出
		float t = static_cast<float>(i) / count;

		// 現在の座標を取得
		now = DirectX::SimpleMath::Vector3::CatmullRom(
			m_startControl, m_start,
			m_end, m_endControl,
			t
		);

		// 描画
		renderer.Draw().Line(prev, now, color);
	}
}
