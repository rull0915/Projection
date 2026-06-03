//====================================================//
// ファイル名   : CheckHit2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要 : 2Dコライダーの衝突判定をまとめたヘッダー
//
// 更新履歴 :
// 2026/04/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include "GameLib/GameObject/Components/Collider/2D/Shapes/2DColliders.h"
#include "../HitInfomation/HitInfomation2D.h"

//====================================================//
// 関数宣言
//====================================================//

/// <summary>
/// 衝突をチェックする関数
/// </summary>
/// <param name="col1">1つ目のコライダー</param>
/// <param name="col2">2つ目のコライダー</param>
/// <returns>衝突しているかどうか</returns>
bool CheckHit2D(BaseCollider2D* colA, BaseCollider2D* colB, HitInfomation2D* info = nullptr);

/// <summary>
/// 円同士の衝突判定
/// </summary>
bool CheckHit2D(CircleCollider2D* colA, CircleCollider2D* colB, HitInfomation2D* info);

/// <summary>
/// 円とカプセル
/// </summary>
bool CheckHit2D(CircleCollider2D* colA, CapsuleCollider2D* colB, HitInfomation2D* info);

/// <summary>
/// 円とボックス
/// </summary>
bool CheckHit2D(CircleCollider2D* colA, BoxCollider2D* colB, HitInfomation2D* info);

/// <summary>
/// カプセル同士
/// </summary>
bool CheckHit2D(CapsuleCollider2D* colA, CapsuleCollider2D* colB, HitInfomation2D* info);

/// <summary>
/// カプセルとボックス
/// </summary>
bool CheckHit2D(CapsuleCollider2D* colA, BoxCollider2D* colB, HitInfomation2D* info);

/// <summary>
/// ボックス同士
/// </summary>
bool CheckHit2D(BoxCollider2D* colA, BoxCollider2D* colB, HitInfomation2D* info);

/// <summary>
/// 凸多角形同士
/// </summary>
bool CheckHit2D(ConvexPolygonCollider2D* colA, ConvexPolygonCollider2D* colB, HitInfomation2D* info);

/// <summary>
/// 円と凸多角形
/// </summary>
bool CheckHit2D(CircleCollider2D* colA, ConvexPolygonCollider2D* colB, HitInfomation2D* info);

/// <summary>
/// カプセルと凸多角形
/// </summary>
bool CheckHit2D(CapsuleCollider2D* colA, ConvexPolygonCollider2D* colB, HitInfomation2D* info);
