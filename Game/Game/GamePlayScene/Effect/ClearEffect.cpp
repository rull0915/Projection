//====================================================//
// ファイル名  : ClearEffect.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/13
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "ClearEffect.h"

#include "Scene/SceneManager.h"
#include "Settings/TimeSettings.h"
#include "GameLib/Transition/SlideTransition.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
ClearEffect::ClearEffect(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_clearUIName{}
	, m_clearDialog{}
	, m_toClearTime{ 1.0f }
	, m_sumTime{ 0 }
	, m_claerTimeScale{ 0.2f }
{
	ADD_PROPERTY(ClearEffect, m_clearUIName);
	ADD_PROPERTY(ClearEffect, m_clearDialog);
	ADD_PROPERTY(ClearEffect, m_toClearTime);
	ADD_PROPERTY(ClearEffect, m_claerTimeScale);
}

// 生成直後に一度呼ばれます
void ClearEffect::Awake()
{
	// スローに
	REngine::TimeSettings::Instance().SetTimeScale(1.0f);
}

// 最初のUpdate関数の直線に一度呼ばれます
void ClearEffect::Start()
{
	// 時間の初期化
	m_sumTime = 0;

	// スローに
	REngine::TimeSettings::Instance().SetTimeScale(m_claerTimeScale);

	// ダイアログを出現
	if (m_clearDialog) m_clearDialog->Show();
}

void ClearEffect::OnDestroy()
{
	// 時間設定を戻す
	REngine::TimeSettings::Instance().SetTimeScale(1.0f);
}

// 毎フレーム呼ばれます
void ClearEffect::Update(const REngine::GameTimer & gameTimer)
{
	// TimeScaleに影響されない時間を加算する
	m_sumTime += gameTimer.GetUnScaledElapsedTime();

	// ボーダーを超えたとき
	if (m_sumTime > m_toClearTime)
	{
		// クリアシーンへの移行をリクエスト
		REngine::SceneManager::Instance().RequestSceneChange("Clear",
			std::make_unique<Transition::Slide>(0.3f, DirectX::SimpleMath::Color{ 0, 0, 0, 1 }, DirectX::XMConvertToRadians(30)),
			std::make_unique<Transition::Slide>(0.3f, DirectX::SimpleMath::Color{ 0, 0, 0, 1 }, DirectX::XMConvertToRadians(210))
			);
	}
}
