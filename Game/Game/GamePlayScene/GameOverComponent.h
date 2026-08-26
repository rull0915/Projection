//====================================================//
// ファイル名   : GameOverComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : ゲームオーバーコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(GameOverComponent)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"
#include "GameObject/GameObject.h"
#include "Player/Player.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class GameOverComponent : public REngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 移行先のシーン名
	std::string m_overSceneName;

	// プレイヤー名
	std::string m_playerName;

	// プレイヤー
	REngine::GameObject* m_player;

	// プレイヤーコンポーネント
	Player* m_playerComponent;

	// ゲームオーバーボーダー
	float m_border3D;
	float m_border2D;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	GameOverComponent(REngine::IComponentOwner* own);
	~GameOverComponent() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------

	COMPONENT_TYPE(GameOverComponent, REngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;
};
