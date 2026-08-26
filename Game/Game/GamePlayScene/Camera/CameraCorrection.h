//====================================================//
// ファイル名   : CameraCorrection.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : カメラ補正コンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(CameraCorrection)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"

#include "GameObject/GameObject.h"
#include "TPSCamera.h"

//====================================================//
// 前方宣言
//====================================================//
class REngine::Scene;

//====================================================//
// クラス宣言
//====================================================//
class CameraCorrection : public REngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------
	
	// カメラ
	TPSCamera* m_camera;

	// プレイヤー
	REngine::GameObject* m_player;

	// プレイヤーのオブジェクト名
	std::string m_playerName;

	// プレイヤーのレイヤー
	int m_playerLayer;

	// シーンポインタ
	REngine::Scene* m_pScene;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	CameraCorrection(REngine::IComponentOwner* own);
	~CameraCorrection() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(CameraCorrection, REngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void LateUpdate(const REngine::GameTimer& gameTimer) override;
};
