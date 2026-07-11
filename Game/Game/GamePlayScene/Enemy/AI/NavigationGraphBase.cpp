//====================================================//
// ファイル名  : NavigationGraphBase.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/24
//
// 概要       :  敵が移動できる有向グラフを作成する基底クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "NavigationGraphBase.h"

#include "Settings/PhysicsSettings.h"

//====================================================//
// 関数の実体宣言
//====================================================//

NavigationGraphBase::NavigationGraphBase(float jumpImpulse, float mass, float horizontalVelocity)
	: m_jumpImpulse{ jumpImpulse }
	, m_enemyMass{ mass }
	, m_horizontalVelocity{ horizontalVelocity }
	, m_maxHeight{ 0.0f }
	, m_gravity{ 0.0f }
	, m_graph{}
{
	// 初速度
	float initV = m_jumpImpulse / m_enemyMass;

	// 重力加速度を取得
	m_gravity = PhysicsSettings::Instance().GetGravityScale();

	// 届く高さの最大値を求める
	m_maxHeight = (initV * initV) / (2 * m_gravity);
}
