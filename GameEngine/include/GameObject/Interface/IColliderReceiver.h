//====================================================//
// ファイル名   : IColliderReceiver.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/28
//
// 概要 : 衝突応答を受け取るインターフェース
//
// 更新履歴 :
// 2026/05/28 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//


//====================================================//
// 前方宣言
//====================================================//
struct HitContact;
struct HitContact2D;

//====================================================//
// クラス宣言
//====================================================//
class IColliderReceiver
{
public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	virtual ~IColliderReceiver() = default;

	// 3D
	virtual void BaseOnCollisionEnter(HitContact& contact) = 0;
	virtual void BaseOnCollisionStay(HitContact& contact) = 0;
	virtual void BaseOnCollisionExit(HitContact& contact) = 0;
	virtual void BaseOnTriggerEnter(HitContact& contact) = 0;
	virtual void BaseOnTriggerStay(HitContact& contact) = 0;
	virtual void BaseOnTriggerExit(HitContact& contact) = 0;

	// 2D
	virtual void BaseOnCollisionEnter2D(HitContact2D& contact) = 0;
	virtual void BaseOnCollisionStay2D(HitContact2D& contact) = 0;
	virtual void BaseOnCollisionExit2D(HitContact2D& contact) = 0;
	virtual void BaseOnTriggerEnter2D(HitContact2D& contact) = 0;
	virtual void BaseOnTriggerStay2D(HitContact2D& contact) = 0;
	virtual void BaseOnTriggerExit2D(HitContact2D& contact) = 0;
};