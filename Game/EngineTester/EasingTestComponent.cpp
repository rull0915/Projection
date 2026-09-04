//====================================================//
// ファイル名  : EasingTestComponent.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/21
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "EasingTestComponent.h"

#include "Renderer/Renderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
EasingTestComponent::EasingTestComponent(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_easingConfig{}
	, m_pTransform{}
	, m_start{ DirectX::SimpleMath::Vector3::Zero }
	, m_end{ DirectX::SimpleMath::Vector3::Zero }
	, m_playScale{ 1.0f }
	, m_sumTime{ 0.0f }
	, m_loop{ false }
	, m_loopInterval{ 0.5f }
{
	ADD_PROPERTY(EasingTestComponent, m_easingConfig);
	ADD_PROPERTY(EasingTestComponent, m_start);
	ADD_PROPERTY(EasingTestComponent, m_end);
	ADD_PROPERTY(EasingTestComponent, m_playScale);
	ADD_PROPERTY(EasingTestComponent, m_loop);
	ADD_PROPERTY(EasingTestComponent, m_loopInterval);
}

// 生成直後に一度呼ばれます
void EasingTestComponent::Awake()
{
	// Transformの取得
	m_pTransform = GetComponent<REngine::Transform>();
}

void EasingTestComponent::OnEnable()
{
	// 経過時間のリセット
	m_sumTime = 0;
}

// 毎フレーム呼ばれます
void EasingTestComponent::Update(const REngine::GameTimer & gameTimer)
{
	// 経過時間の加算
	m_sumTime += gameTimer.GetElapsedTime() * m_playScale;

	// 補間後の値を取得
	float finalT = m_easingConfig.Get(m_sumTime);

	// 開始点から終了点へ向かうベクトル
	DirectX::SimpleMath::Vector3 v = m_end - m_start;

	// 次の位置
	DirectX::SimpleMath::Vector3 next = m_start + finalT * v;

	// 位置を設定
	m_pTransform->SetWorldPosition(next);

	// ループの時
	if (m_loop)
	{
		// インターバルを超えたら
		if (m_sumTime > 1.0f + m_loopInterval)
		{
			// 経過時間をリセット
			m_sumTime = 0;
		}
	}
}

void EasingTestComponent::DebugRender(REngine::Renderer& renderer, const DirectX::SimpleMath::Color& color)
{
	// 開始点から終了点への線を引く
	renderer.Draw().Primitive().DrawLine(m_start, m_end, color);
}
