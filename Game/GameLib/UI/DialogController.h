//====================================================//
// ファイル名   : DialogController.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/30
//
// 概要 : ダイアログコンポーネントを操作するコンポーネント
//
// 更新履歴 :
// 2026/08/30 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(DialogController)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/Both/BothComponentBase.h"
#include "Dialog.h"
#include "Components/UI/Behaviour/ButtonUI.h"

//====================================================//
// クラス宣言
//====================================================//
class DialogController : public RHEngine::BothComponentBase
{
public:

	// クリック時の操作
	enum class ClickedOperation
	{
		Open, Close
	};

private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 操作対象のDialog
	RHEngine::Ref<Dialog> m_targetDialog;

	// クリック時の操作
	ClickedOperation m_clickedOperation;

	// 開くボタン
	RHEngine::Ref<RHEngine::ButtonUI> m_openButton;
	RHEngine::Ref<RHEngine::ButtonUI> m_closeButton;

	// 削除用ID
	int m_openId;
	int m_closeId;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	DialogController(RHEngine::IComponentOwner* own);
	~DialogController() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(MyComponent, RHEngine::BothComponentBase);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void OnDestroy() override;
};
