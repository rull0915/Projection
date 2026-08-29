//====================================================//
// ファイル名   : ClearEffect.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : クリア時の演出、シーン遷移を行うコンポーネント
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(ClearEffect)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IComponentOwner.h"
#include "GameObject/GameObject.h"

//====================================================//
// 前方宣言
//====================================================//


//====================================================//
// クラス宣言
//====================================================//
class ClearEffect : public REngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// クリアUIの名前
	std::string m_clearUIName;

	// クリアUI
	REngine::GameObject* m_clearUI;

	// クリアシーンへ移行するまでの時間
	float m_toClearTime;

	// 現在の時間
	float m_sumTime;

	// クリア時のタイムスケール
	float m_claerTimeScale;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ClearEffect(REngine::IComponentOwner* own);
	~ClearEffect() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(ClearEffect, REngine::WorldComponentBase)

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Awake() override;
	void Start() override;

	void OnDestroy() override;

	void Update(const REngine::GameTimer& gameTimer) override;
};
