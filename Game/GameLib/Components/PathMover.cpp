//====================================================//
// ファイル名  : PathMover.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/07/11
//
// 概要       : パスを使用し移動するコンポーネント
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "PathMover.h"

#include "Math/GameMath.h"

//====================================================//
// 関数の実体宣言
//====================================================//

// コンストラクタ
PathMover::PathMover(REngine::IComponentOwner* own)
	: WorldComponentBase(own)
	, m_magnification{ 1.0f }
	, m_playOnStart{ false }
	, m_isPlaying{ false }
	, m_sumTime{ 0 }
	, m_pTransform{ nullptr }
	, m_pCatmull{ nullptr }
	, m_rotateType{ 0 }
{
	ADD_PROPERTY(PathMover, m_magnification);
	ADD_PROPERTY(PathMover, m_playOnStart);
	ADD_PROPERTY(PathMover, m_rotateType);
}

// 最初のUpdate関数の直線に一度呼ばれます
void PathMover::Start()
{
	// トランスフォームの取得
	m_pTransform = GetComponent<REngine::Transform>();

	// 補間コンポーネントの取得
	m_pCatmull = GetComponent<CatmullRomComponent>();

	// 開始時実行フラグがオンなら
	if (m_playOnStart)
	{
		// スタート
		MoveStart();
	}
}

// 毎フレーム呼ばれます
void PathMover::Update(const REngine::GameTimer & gameTimer)
{
	// プレイ中なら
	if (m_isPlaying)
	{
		// 経過時間に倍率を掛けて加算する
		m_sumTime += gameTimer.GetElapsedTime() * m_magnification;
	}
}

// 毎フレームUpdate及び物理挙動の後に呼ばれます
void PathMover::LateUpdate(const REngine::GameTimer & gameTimer)
{
	gameTimer;

	if (m_pCatmull)
	{
		// 位置を計算
		DirectX::SimpleMath::Vector3 nextPos = m_pCatmull->GetValue(m_sumTime);

		// 回転の補正
		switch (m_rotateType)
		{
			// 進行方向への補正の場合
		case RotType::Forward: {

			// 進行方向ベクトルを算出
			DirectX::SimpleMath::Vector3 dir = m_pCatmull->GetTangent(m_sumTime);

			// 前方向が進行方向になるようなクォータニオンを作成する
			DirectX::SimpleMath::Quaternion q = REngine::Math::FromToRotation(DirectX::SimpleMath::Vector3::Forward, dir);

			// セット
			m_pTransform->SetLocalRotation(q);
		}
			  break;
		case RotType::Rigit:{

			// 進行方向ベクトルを算出
			DirectX::SimpleMath::Vector3 dir = m_pCatmull->GetTangent(m_sumTime);

			// 右方向を作成
			DirectX::SimpleMath::Vector3 right = dir.Cross(DirectX::SimpleMath::Vector3::Up);

			// 前方向が進行方向の右方向方向になるようなクォータニオンを作成する
			DirectX::SimpleMath::Quaternion q = REngine::Math::FromToRotation(DirectX::SimpleMath::Vector3::Forward, right);

			// セット
			m_pTransform->SetLocalRotation(q);
		}
			break;
		case RotType::Left:{

			// 進行方向ベクトルを算出
			DirectX::SimpleMath::Vector3 dir = m_pCatmull->GetTangent(m_sumTime);

			// 右方向を作成
			DirectX::SimpleMath::Vector3 left = DirectX::SimpleMath::Vector3::Up.Cross(dir);

			// 前方向が進行方向の右方向方向になるようなクォータニオンを作成する
			DirectX::SimpleMath::Quaternion q = REngine::Math::FromToRotation(DirectX::SimpleMath::Vector3::Forward, left);

			// セット
			m_pTransform->SetLocalRotation(q);
		}
			break;
		default:
			break;
		}

		// セット
		m_pTransform->SetWorldPosition(nextPos);
	}
}

void PathMover::MoveStart()
{
	// フラグのセット
	m_isPlaying = true;

	// タイマーのリセット
	m_sumTime = 0;
}
