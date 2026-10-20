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

#define IS_COMPONENT(PlaySceneManager)

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "ChangeDimention/DimentionManager.h"
#include "GameObject/GameObject.h"

#include "Common/EventBus.h"
#include "GamePlayEvent.h"

//====================================================//
// 前方宣言
//====================================================//
class Canvas;

//====================================================//
// クラス宣言
//====================================================//
class PlaySceneManager : public RHEngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// カメラ
	RHEngine::Ref<RHEngine::GameObject> m_camera;

	// プレイヤー
	RHEngine::Ref<RHEngine::GameObject> m_player;

	// 次元管理
	RHEngine::Ref<DimentionManager> m_dimentionManager;

	// イベントのID
	std::vector<RHEngine::EventBus<GamePlayEvent>::Token> m_eventIds;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	PlaySceneManager(RHEngine::IComponentOwner* own);
	~PlaySceneManager() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------

	COMPONENT_TYPE(PlaySceneManager, RHEngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;

	void OnDestroy() override;

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;

private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 次元の切り替えを試みる関数
	void TryChangeDimention();
};
