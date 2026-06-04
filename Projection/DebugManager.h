//====================================================//
// ファイル名   : DebugManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/04
//
// 概要 : デバッグの設定クラス
//
// 更新履歴 :
// 2026/06/04 新規作成
//====================================================//

#pragma once

//====================================================//
// クラス宣言
//====================================================//
class DebugManager
{
private:
    static constexpr float STEP_INTERVAL = 0.05f;    // 連続ステップ実行時の実行間隔
    static constexpr float STEP_BORDER = 0.5f;      // 連続ステップ実行へ移行する長押し時間のボーダー

private:
    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // フラグ
    bool gameStop;      // 停止フラグ
    bool stepUpdate;    // 1度のみ更新するフラグ
    bool drawColliderDebug; // コライダーのデバッグ表示をするフラグ
    bool drawColliderDebugAABB; // コライダーのAABBを表示するフラグ
    bool drawRectTransforn;     // レクトトランスフォームの範囲を表示するフラグ

    // その他補助変数
    float stepLongPressTime;
    bool stepContinuous;    // 連続実行フラグ

private:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    DebugManager()
        : gameStop{ false }
        , stepUpdate{ false }
        , drawColliderDebug{ false }
        , drawColliderDebugAABB{ false }
        , drawRectTransforn{ false }
        , stepContinuous{ false }
        , stepLongPressTime{ 0.0f }
    {
    }
    ~DebugManager() {};

public:
    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    // シングルトン化
    static DebugManager& Instance()
    {
        static DebugManager instance;
        return instance;
    }

    void Update(float elapsedTime);

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

	bool IsGameStop() const { return gameStop; }
	bool IsStepUpdate() const { return stepUpdate; }
	bool IsDrawColliderDebug() const { return drawColliderDebug; }
	bool IsDrawColliderDebugAABB() const { return drawColliderDebugAABB; }
	bool IsDrawRectTransform() const { return drawRectTransforn; }
};