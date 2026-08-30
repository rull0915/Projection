//====================================================//
// ファイル名   : PauseDialog.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/30
//
// 概要 : ポーズ画面のダイアログ
//
// 更新履歴 :
// 2026/08/30 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(PauseDialog)

//====================================================//
// インクルードファイル
//====================================================//
#include "Dialog.h"
#include "Components/UI/RectTransform/RectTransform.h"

#include "EngineTester/EasingConfig.h"

//====================================================//
// クラス宣言
//====================================================//
class PauseDialog : public Dialog
{
public:
	// 拡大モード
	enum class Expansion
	{
		X, Y, XY
	};

private:
	// ダイアログのモード
	enum class Mode
	{
		Show, Hide, Showing, Hiding
	};

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// RectTransform
	REngine::RectTransform* m_rectTransform;

	// 使用するイージング
	EasingConfig m_openEasing;
	EasingConfig m_closeEasing;

	// 拡大モード
	Expansion m_expType;

	// 開閉にかける時間
	float m_transTime;

	// 初期状態のスケール
	DirectX::SimpleMath::Vector2 m_initScale;

	// 現在のモード
	Mode m_currentMode;

	// 今の経過時間
	float m_currentTime;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	PauseDialog(REngine::IComponentOwner* own);
	~PauseDialog() = default;

	//-----------------------------------------------------
	// Type
	//-----------------------------------------------------
	
	COMPONENT_TYPE(PauseDialog, Dialog);
	
	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void Start() override;

	void Update(const REngine::GameTimer& gameTimer) override;

	// 開く
	void Show() override;

	// 閉じる
	void Hide() override;

private:
		
	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// RectTransformのスケールを変更する関数
	void ChangeScale(float ratio);
};
