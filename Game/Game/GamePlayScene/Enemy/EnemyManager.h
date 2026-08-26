//====================================================//
// ファイル名   : EnemyManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/17
//
// 概要 : 敵管理クラス
//
// 更新履歴 :
// 2026/06/17 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(EnemyManager)

//====================================================//
// インクルードファイル
//====================================================//
#include "AI/NavigationGraph.h"
#include "AI/NavigationGraph2D.h"

#include "Enemy.h"
#include "Components/World/Transform/Transform.h"

#include "Components/World/WorldComponentBase.h"
#include "Components/Interface/IComponentListener.h"

#include "Common/EventBus.h"
#include "../GamePlayEvent.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class EnemyManager : public REngine::WorldComponentBase, public  REngine::IComponentListener
{
	// グラフを更新する感覚
	static constexpr float GRAPH_UPDATE_DISTANCE = 0.2f;

	// 道を再計算するボーダー(距離の2乗)
	static constexpr float WAY_UPDATE_BORDER = 0.0f;
	
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 通常の敵が使用するグラフ
	NavigationGraph m_normalNavigation;
	NavigationGraph2D m_normalNavigation2D;

	// 管理している敵リスト
	std::vector<Enemy*> m_enemies;
	std::vector<Enemy*> m_addReserves;
	std::vector<Enemy*> m_removeReserves;

	// 時間管理
	float m_nowTime;

	// プレイヤーのトランスフォーム
	REngine::Transform* m_playerTransform;

	// プレイヤーがいた位置
	DirectX::SimpleMath::Vector3 m_oldPlayerPosition;

	// 2Dフラグ
	bool m_is2D;

	// テスト用
	std::vector<NavigationGraphBase::Edge> m_debugPath;

	// スタートフラグ
	bool m_isStarted;

	// イベントのトークン
	REngine::EventBus<GamePlayEvent>::Token m_eventToken;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	EnemyManager(REngine::IComponentOwner* owner)
		: WorldComponentBase(owner)
		, m_normalNavigation{ Enemy::JUMP_IMPLUSE, 1.0f, Enemy::VELOCITY }
		, m_normalNavigation2D{ Enemy::JUMP_IMPLUSE, 1.0f, Enemy::VELOCITY }
		, m_enemies{}
		, m_nowTime{ 0 }
		, m_playerTransform{ nullptr }
		, m_is2D{ false }
		, m_isStarted{ false }
		, m_eventToken{}
	{
	}
	~EnemyManager() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(EnemyManager, REngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------
	void Awake() override;

	void Start() override;

	void OnDestroy() override;

	void Update(const REngine::GameTimer& timer) override;

	// 敵を追加する関数
	void OnComponentAdded(ComponentBase* component) override
	{
		// 敵なら
		if (component->IsTypeOf(Enemy::StaticTypeId()))
		{
			Enemy* enemy = static_cast<Enemy*>(component);

			if (!m_isStarted) enemy->SetActive(false);

			// キャストして追加
			m_addReserves.push_back(enemy);
		}

		// 着地候補点なら
		if (component->IsTypeOf(LandingCandidatePoints::StaticTypeId()))
			// キャストして追加
			m_normalNavigation.AddNode(static_cast<LandingCandidatePoints*>(component));
		
		// 2D着地候補点なら
		if (component->IsTypeOf(LandingCandidatePoints2D::StaticTypeId()))
			// キャストして追加
			m_normalNavigation2D.AddNode(static_cast<LandingCandidatePoints2D*>(component));
	}
	// 敵を削除する関数
	void OnComponentRemoved(ComponentBase* component) override
	{
		// 敵なら
		if (component->IsTypeOf(Enemy::StaticTypeId()))
			// キャストして追加
			m_removeReserves.push_back(static_cast<Enemy*>(component));	

		// 着地候補点なら
		if (component->IsTypeOf(LandingCandidatePoints::StaticTypeId()))
			// キャストして追加
			m_normalNavigation.RemoveNode(static_cast<LandingCandidatePoints*>(component));

		// 2D着地候補点なら
		if (component->IsTypeOf(LandingCandidatePoints2D::StaticTypeId()))
			// キャストして追加
			m_normalNavigation2D.RemoveNode(static_cast<LandingCandidatePoints2D*>(component));
	}

	// 敵の次元変更を行う関数
	void ChangeDimantion();

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 追加予約済みの敵を追加する
	void AddReserved()
	{
		for (auto& component : m_addReserves)
		{
			m_enemies.push_back(component);

			// フラグを自分と合わせる
			component->SetIs2D(m_is2D);
		}

		// リセット
		m_addReserves.clear();
	}

	// 削除予約済みの敵を削除する
	void RemoveReserved()
	{
		for (auto& component : m_removeReserves)
		{
			// 配列にあれば削除する
			m_enemies.erase(
				std::remove(m_enemies.begin(), m_enemies.end(), component),
				m_enemies.end()
			);
		}

		// リセット
		m_removeReserves.clear();
	}
};
