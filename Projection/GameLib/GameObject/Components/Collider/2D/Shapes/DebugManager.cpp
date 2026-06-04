//====================================================//
// ファイル名  : DebugManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/04
//
// 概要       : デバッグ管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "DebugManager.h"

#include "GameLib/Input/KeyInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

void DebugManager::Update(float elapsedTime)
{
	// デバッグビルドなら
#ifdef _DEBUG

	// リセットする必要のあるフラグをリセット
	stepUpdate = false;

	// 入力チェック

	// F1キー
	if (KeyInput::GetKeyDown(KeyCode::F1))
	{
		// ゲームの再生停止切り替え
		gameStop = !gameStop;
	}
	// F2キー
	if (KeyInput::GetKeyDown(KeyCode::F2))
	{
		// ゲームが停止中なら
		if (gameStop)
		{
			// ステップ実行フラグをオンに
			stepUpdate = true;
		}
	}
	if (KeyInput::GetKey(KeyCode::F2))
	{
		if (gameStop)
		{
			// 時間を加算
			stepLongPressTime += elapsedTime;

			if (!stepContinuous)
			{
				// ボーダーを超えたら連続実行へ移行
				if (stepLongPressTime > STEP_BORDER)
				{
					stepContinuous = true;
					stepLongPressTime = 0.0f;
				}
			}
			else
			{
				// インターバルを超えたらフラグをオン
				if (stepLongPressTime > STEP_INTERVAL)
				{
					stepUpdate = true;
					stepLongPressTime = 0.0f;
				}
			}
		}
	}
	if (KeyInput::GetKeyUp(KeyCode::F2))
	{
		stepContinuous = false;
		stepLongPressTime = 0.0f;
	}
	// F3キー
	if (KeyInput::GetKeyDown(KeyCode::F3))
	{
		// コライダーの表示を管理
		
		// 非表示の時
		if (!drawColliderDebug) drawColliderDebug = true;

		// AABBのみ非表示の時
		else if (!drawColliderDebugAABB)
		{
			drawColliderDebugAABB = true;
		}
		// どちらの表示されているとき
		else
		{
			drawColliderDebug = false;
			drawColliderDebugAABB = false;
		}
	}
	// F4キー
	if (KeyInput::GetKeyDown(KeyCode::F4))
	{
		drawRectTransforn = !drawRectTransforn;
	}

#endif 	
}
