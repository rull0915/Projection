//====================================================//
// ファイル名  : DimentionManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/06
//
// 概要       : 次元変更管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "DimentionManager.h"
#include "Settings/WorldSetting2D.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
DimentionManager::DimentionManager()
	: m_pCamera{ nullptr }
	, m_components{}
	, m_addReserves{}
	, m_removeReserves{}
	, m_nowState{ State::World3D }
{
}

void DimentionManager::Initialize()
{
	m_nowState = State::World3D;

	m_components.clear();

	m_addReserves.clear();
	m_removeReserves.clear();
}

void DimentionManager::Update()
{
	// 今の状態によって分岐
	switch (m_nowState)
	{
		// 2Dへ変更中の時
	case DimentionManager::ChangeTo2D:

		// カメラの変更が終わったら
		if (m_pCamera && !m_pCamera->IsChanging())
		{
			// 世界を変更
			WorldTo2D();

			m_nowState = State::World2D;
		}

		break;
	case DimentionManager::ChangeTo3D:

		// カメラの変更が終わったら
		if (m_pCamera && !m_pCamera->IsChanging())
		{
			m_nowState = State::World3D;
		}

		break;
	default:
		break;
	}
}

void DimentionManager::ChangeDimention(float changeTime)
{
	// カメラが設定されてなければ何もしない
	if (!m_pCamera) return;

	// 切り替え中なら何もしない
	if (m_pCamera->IsChanging()) return;

	// 追加予約済みのコンポーネントを追加する
	AddReserved();

	// 削除予約済みのコンポーネントを削除する
	RemoveReserved();

	// カメラの投影モードを変更
	m_pCamera->ChangeProjectionMode(changeTime);

	// 状態の変更
	switch (m_nowState)
	{
	case State::World2D:
		m_nowState = State::ChangeTo3D;
		WorldTo3D();
		break;
	case State::World3D:
		m_nowState = State::ChangeTo2D;
		break;
	default:
		break;
	}
}

void DimentionManager::WorldTo2D()
{
	// 2次元世界の軸方向をカメラ基準に変更
	WorldSetting2D::Instance().SetAxis(m_pCamera->GetInverseView().Right(), m_pCamera->GetInverseView().Up());

	// 管理しているコンポーネントの切り替え関数を呼び出す
	for (auto& component : m_components)
	{
		component->Change3DTo2D(m_pCamera);
	}
}

void DimentionManager::WorldTo3D()
{
	// 管理しているコンポーネントの切り替え関数を呼び出す
	for (auto& component : m_components)
	{
		component->Change2DTo3D();
	}
}
