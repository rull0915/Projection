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

//====================================================//
// クラス宣言
//====================================================//
class TitleManager : public REngine::WorldComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 遷移対象のシーン名
	std::string m_targetScene;

	// プレイボタン
	REngine::Ref<REngine::ButtonUI> m_playButton;

	// 終了ボタン
	REngine::Ref<REngine::ButtonUI> m_exitButton;
	REngine::Ref<REngine::ButtonUI> m_exitAcceptButton;
	REngine::Ref<REngine::ButtonUI> m_exitCancelButton;

	// 追加したイベント
	REngine::ButtonUI::EventToken m_playEventToken;
	REngine::ButtonUI::EventToken m_exitEventToken;

	// コントローラーが接続されているときのみ表示されるUI
	REngine::Ref<REngine::GameObject> m_padUI;
	REngine::Ref<REngine::GameObject> m_padUIOnDialog;

	// 終了ダイアログ
	REngine::Ref<Dialog> m_quitDialog;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	TitleManager(REngine::IComponentOwner* own);
	~TitleManager() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(TitleManager, REngine::WorldComponentBase);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;

	void OnDestroy() override;
};
