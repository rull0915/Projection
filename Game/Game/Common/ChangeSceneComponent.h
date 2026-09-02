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

//====================================================//
// クラス宣言
//====================================================//
class ChangeSceneComponent : public REngine::BothComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// ボタン
	REngine::Ref<REngine::ButtonUI> m_button;

	// 遷移先のシーン名
	std::string m_targetScene;

	// イベントトークン
	int m_eventToken;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ChangeSceneComponent(REngine::IComponentOwner* own);
	~ChangeSceneComponent() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(ChangeSceneComponent, REngine::BothComponentBase);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void OnDestroy() override;
};
