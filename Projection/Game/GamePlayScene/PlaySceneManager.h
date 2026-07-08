//====================================================//
// ファイル名   : PlaySceneManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : ゲームプレイシーンの管理コンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "Enemy/EnemyManager.h"
#include "ChangeDimention/DimentionManager.h"
#include "GameObject/GameObject.h"

//====================================================//
// 前方宣言
//====================================================//
class Canvas;

//====================================================//
// クラス宣言
//====================================================//
class PlaySceneManager : public WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 検索に使用する名前
	std::string m_playerName;
	std::string m_cameraName;
	std::string m_clearUIName;

	// カメラ
	GameObject* m_camera;

	// プレイヤー
	GameObject* m_player;

	// 次元管理
	DimentionManager* m_dimentionManager;

	// 敵管理
	EnemyManager* m_enemyManager;

	// クリアUI
	GameObject* m_clearUI;

	// イベントのID
	unsigned int m_eventID;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	PlaySceneManager(IComponentOwner* own);
	~PlaySceneManager() = default;

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;

	void OnDestroy() override;

	void Start() override;

	void Update(const GameTimer& gameTimer) override;

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<PlaySceneManager>();
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 次元の切り替えを試みる関数
	void TryChangeDimention();
};
