//====================================================//
// ファイル名   : ChangeSceneComponent.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/09/02
//
// 概要 : シーンを変更するコンポーネント
//
// 更新履歴 :
// 2026/09/02 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(ChangeSceneComponent)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/Both/BothComponentBase.h"
#include "Components/UI/Behaviour/ButtonUI.h"
#include "GameObject/GameObject.h"

//====================================================//
// クラス宣言
//====================================================//
class ChangeSceneComponent : public RHEngine::BothComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// ボタン
	RHEngine::Ref<RHEngine::ButtonUI> m_button;

	// コントローラーの操作UI
	RHEngine::Ref<RHEngine::GameObject> m_padUI;

	// 遷移先のシーン名
	std::string m_targetScene;

	// イベントトークン
	int m_eventToken;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ChangeSceneComponent(RHEngine::IComponentOwner* own);
	~ChangeSceneComponent() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(ChangeSceneComponent, RHEngine::BothComponentBase);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void Update(const RHEngine::GameTimer& timer) override;

	void OnDestroy() override;
};
