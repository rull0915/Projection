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
#include "GameLib/GameObject/GameObject.h"

#include "GameLib/Scene/Scene.h"

#include "GameLib/GameObject/Components/Components.h"
#include "Player/Player.h"
#include "ChangeDimention/ChangeColliderComponent.h"
#include "Enemy/Enemy.h"
#include "Enemy/Components/LandingCandidatePoints.h"

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
		obj->AddComponent<CapsuleCollider>()->SetLayer(10);

		// 着地判定用のコライダー
		auto land = obj->AddComponent<BoxCollider>();// 着地判定用のコライダーを生成
		land->SetTrigger(true);
		land->SetLocalPos({ 0, -1.05f, 0 });
		land->SetLocalSize({ 1, 0.1f, 1 });

		// モデル描画
		obj->AddComponent<ModelComponent>()->SetModel("Template_Capsule");

		// 物理挙動
		obj->AddComponent<RigidBody>();

		// 次元変化
		obj->AddComponent<ChangeColliderComponent>();

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
		cube->AddComponent<ModelComponent>()->SetModel("Template_Cube");

		// トランスフォームの設定
		cube->GetComponent<Transform>()->SetLocalScale(scale);
		cube->GetComponent<Transform>()->SetLocalEulerAngle({ rot });

		// 次元変化
		cube->AddComponent<ChangeColliderComponent>();

		// 着地候補として設定
		cube->AddComponent<LandingCandidatePoints>();

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

		// 物理挙動
		enemy->AddComponent<RigidBody>();

		// コライダー
		enemy->AddComponent<CapsuleCollider>();

		// 次元変化

		// 敵
		enemy->AddComponent<Enemy>();

		return enemy;
	}
};