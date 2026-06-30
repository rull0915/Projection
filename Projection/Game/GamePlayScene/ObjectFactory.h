//====================================================//
// ファイル名   : ObjectFactory.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/22
//
// 概要 : 特定の種類のオブジェクトの生成、コンポーネントの追加などを管理するヘッダ
//
// 更新履歴 :
// 2026/06/22 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "GameObject/GameObject.h"

#include "Scene/Scene.h"

#include "Components/World/Components.h"
#include "Player/Player.h"
#include "ChangeDimention/ChangeColliderComponent.h"
#include "ChangeDimention/DepthCorrection.h"
#include "Enemy/Enemy.h"
#include "Enemy/Components/LandingCandidatePoints.h"
#include "Enemy/Components/LandingCandidatePoints2D.h"
#include "Stage/Components/GoalComponent.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
namespace ObjectFactory
{
	// プレイヤーの生成
	GameObject* CreatePlayer(Scene* scene, DirectX::SimpleMath::Vector3 position = { 0, 0, 0 })
	{
		// 原型を生成
		GameObject* obj = scene->Generate(position);

		// 必須コンポーネントを追加

		// プレイヤー
		obj->AddComponent<Player>();

		// コライダー
		auto bottom = obj->AddComponent<BoxCollider>();
		bottom->SetLayer(10);
		bottom->SetLocalSize({ 1, 1.6f, 1 });

		// コライダー
		auto top = obj->AddComponent<BoxCollider>();
		top->SetLayer(10);
		top->SetLocalPos({ 0, 1.6f, 0 });
		top->SetLocalSize({ 1.6f, 1.6f, 1.6f });

		// 着地判定用のコライダー
		auto land = obj->AddComponent<BoxCollider>();// 着地判定用のコライダーを生成
		land->SetTrigger(true);
		land->SetNeedInfo(true);
		land->SetLocalPos({ 0, -0.85f, 0 });
		land->SetLocalSize({ 1, 0.1f, 1 });

		auto transform = obj->GetComponent<Transform>();
		transform->SetLocalScale({ 0.4f, 0.4f, 0.4f });

		// モデル描画
		obj->AddComponent<ModelComponent>()->SetModel("Player");

		// 物理挙動
		obj->AddComponent<RigidBody>();

		// 次元変化
		obj->AddComponent<ChangeColliderComponent>();

		// 2D補正
		obj->AddComponent<DepthCorrection>();

		// 生成したオブジェクトを返す
		return obj;
	}

	// ブロックの生成
	GameObject* CreateCube(Scene* scene, DirectX::SimpleMath::Vector3 position = { 0, 0, 0 }, DirectX::SimpleMath::Vector3 rot = { 0, 0, 0 }, DirectX::SimpleMath::Vector3 scale = { 1, 1, 1 })
	{
		// 原型を生成
		auto cube = scene->Generate(position);

		// コライダー
		cube->AddComponent<BoxCollider>();

		// モデル描画
		cube->AddComponent<ModelComponent>()->SetModel("Cube");

		// トランスフォームの設定
		cube->GetComponent<Transform>()->SetLocalScale(scale);
		cube->GetComponent<Transform>()->SetLocalEulerAngle({ rot });

		// 次元変化
		cube->AddComponent<ChangeColliderComponent>();

		// 着地候補として設定
		cube->AddComponent<LandingCandidatePoints>();
		cube->AddComponent<LandingCandidatePoints2D>();

		// タグの設定
		cube->SetTag("Floor");

		return cube;
	}

	// 敵の生成
	GameObject* CreateEnemy(Scene* scene, DirectX::SimpleMath::Vector3 position = { 0, 0, 0 })
	{
		// 原型の生成
		GameObject* enemy = scene->Generate(position);

		// コンポーネントの追加

		// モデル
		enemy->AddComponent<ModelComponent>()->SetModel("Enemy");

		// 物理挙動
		enemy->AddComponent<RigidBody>();

		// コライダー
		auto col = enemy->AddComponent<SphereCollider>();
		//col->SetHeight(1.5f);
		col->SetLocalPos({ 0, 0.5f, 0 });

		// 次元変化
		enemy->AddComponent<ChangeColliderComponent>();

		// 2D補正
		enemy->AddComponent<DepthCorrection>();

		// 敵
		enemy->AddComponent<Enemy>();

		return enemy;
	}

	// ゴールオブジェクトの生成
	GameObject* CreateGoal(Scene* scene, DirectX::SimpleMath::Vector3 position = { 0, 0, 0 })
	{
		// 原型の生成
		GameObject* goal = scene->Generate(position);

		// コンポーネントの追加

		// 設定
		auto t = goal->GetComponent<Transform>();
		t->SetLocalScale({ 0.3f, 0.3f, 0.3f });

		// モデル
		goal->AddComponent<ModelComponent>()->SetModel("Goal");

		// コライダー
		auto col = goal->AddComponent<BoxCollider>();
		col->SetLocalPos({ 0, 0, 0 });
		col->SetLocalSize({ 3, 3, 3 });
		col->SetTrigger(true);

		// 敵
		goal->AddComponent<GoalComponent>();

		return goal;
	}
};
