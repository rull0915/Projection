//====================================================//
// ファイル名   : TitleManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/09/01
//
// 概要 : 
//
// 更新履歴 :
// 2026/09/01 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(TitleManager)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Components/UI/Behaviour/ButtonUI.h"
#include "GameObject/GameObject.h"
#include "GameLib/UI/Dialog.h"
#include "Components/Both/Sounds/AudioSource.h"

//====================================================//
// クラス宣言
//====================================================//
class TitleManager : public RHEngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 遷移対象のシーン名
	std::string m_targetScene;

	// プレイボタン
	RHEngine::Ref<RHEngine::ButtonUI> m_playButton;

	// 終了ボタン
	RHEngine::Ref<RHEngine::ButtonUI> m_exitButton;
	RHEngine::Ref<RHEngine::ButtonUI> m_exitAcceptButton;
	RHEngine::Ref<RHEngine::ButtonUI> m_exitCancelButton;

	// 追加したイベント
	RHEngine::ButtonUI::EventToken m_playEventToken;
	RHEngine::ButtonUI::EventToken m_exitEventToken;

	// コントローラーが接続されているときのみ表示されるUI
	RHEngine::Ref<RHEngine::GameObject> m_padUI;
	RHEngine::Ref<RHEngine::GameObject> m_padUIOnDialog;

	// 終了ダイアログ
	RHEngine::Ref<Dialog> m_quitDialog;

	// 音楽
	RHEngine::Ref<RHEngine::AudioSource> m_audioSource;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	TitleManager(RHEngine::IComponentOwner* own);
	~TitleManager() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(TitleManager, RHEngine::WorldComponentBase);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void Update(const RHEngine::GameTimer& gameTimer) override;

	void OnDestroy() override;
};
