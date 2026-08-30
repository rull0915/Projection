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
#include "Components/UI/Behaviour/ButtonListenerBase.h"
#include "Dialog.h"

//====================================================//
// クラス宣言
//====================================================//
class DialogController : public REngine::ButtonListenerBase
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
	REngine::Ref<Dialog> m_targetDialog;

	// クリック時の操作
	ClickedOperation m_clickedOperation;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	DialogController(REngine::IComponentOwner* own);
	~DialogController() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(MyComponent, REngine::ButtonListenerBase);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void OnClicked() override;
};
