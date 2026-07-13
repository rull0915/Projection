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

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Settings/TimeSettings.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
ClearEffect::ClearEffect(IComponentOwner* own)
	: WorldComponentBase(own)
	, m_clearUIName{}
	, m_clearUI{ nullptr }
	, m_toClearTime{ 1.0f }
	, m_sumTime{ 0 }
	, m_claerTimeScale{ 0.2f }
{
	ADD_PROPERTY(m_clearUIName);
	ADD_PROPERTY(m_toClearTime);
	ADD_PROPERTY(m_claerTimeScale);
}

// 生成直後に一度呼ばれます
void ClearEffect::Awake()
{}

// 最初のUpdate関数の直線に一度呼ばれます
void ClearEffect::Start()
{
	// シーンを取得
	Scene* scene = static_cast<GameObject*>(GetOwn())->GetScene();

	// クリアUIを取得
	m_clearUI = scene->GetObjectFinder()->FindWithNameInUI(m_clearUIName);

	// 時間の初期化
	m_sumTime = 0;

	// スローに
	TimeSettings::Instance().SetTimeScale(m_claerTimeScale);
}

void ClearEffect::OnDestroy()
{
	
}

// 毎フレーム呼ばれます
void ClearEffect::Update(const GameTimer & gameTimer)
{
	// TimeScaleに影響されない時間を加算する
	m_sumTime += gameTimer.GetUnScaledElapsedTime();

	// ボーダーを超えたとき
	if (m_sumTime > m_toClearTime)
	{
		// クリアシーンへの移行をリクエスト
		SceneManager::Instance().RequestSceneChange("Clear");
	}
}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void ClearEffect::LateUpdate(const GameTimer & gameTimer)
{}
