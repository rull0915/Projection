//====================================================//
// ファイル名   : ComponentID.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/22
//
// 概要 : コンポーネントのIDを宣言したヘッダ
//			コンポーネントを追加する際はこのヘッダに対応する列挙子を追加して下さい。
//			他のコンポーネントとかぶりがないようにして下さい。
// 
//			上位8bitでメインID、下位8bitでサブIDを定義します。
//			0x0102の場合、メイン01 サブ02のコンポーネントとなります。
//			メインIDが等しいコンポーネントは、GetComponentをした際にまとめて取得することが出来ます。
//
// 更新履歴 :
// 2026/04/22 新規作成
//====================================================//

#pragma once

//====================================================//
// マクロ宣言
//====================================================//

constexpr uint16_t MAIN_MASK = (0xFF00);
constexpr uint16_t SUB_MASK = (0xFFFF);

// コンポーネントを識別するID列挙型
enum class ComponentID
{
	//========== 標準ID =========//

	// トランスフォーム
	Transform	= 0x0100,

	// 3Dコライダー
	Collider	= 0x0200,
		BoxCollider		= 0x0201,
		SphereCollider	= 0x0202,
		CapsuleCollider	= 0x0203,

	// 2Dコライダー
	Collider2D	= 0x0300,
		BoxCollider2D			= 0x0301,
		CircleCollider2D		= 0x0302,
		CapsuleCollider2D		= 0x0303,
		ConvexPolygonCollider2D	= 0x0304,

	// 3D物理挙動
	RigidBody	= 0x0400,

	// 2D物理挙動
	RigidBody2D	= 0x0500,

	// 描画
	Renderer	= 0x0600,
		ModelRenderer	= 0x0601,
		SpriteRenderer	= 0x0602,

	// カメラ
	Camera		= 0x0700,
		StandardCamera	= 0x0701,
		TargetCamera	= 0x0702,

	//========== カスタムID =========//
	// プロジェクト独自のコンポーネントここから下に書くようにして下さい。

	// コライダーの切り替えを行うコンポーネント
	ChangeColliderComponent = 0x1000,
	// プレイヤーに着けるコンポーネント
	Player					= 0x1100,
	// カメラをTPS用に動かすコンポーネント
	TPSCamera				= 0x1200,
	// プロジェクション行列をスムーズに切り替えるカメラ
	ProjectionSmoothCamera	= 0x0703,
	// 着地候補点
	LandingCandidatePoints	= 0x1300
};

// MAIN_IDのみを切り取る関数
static constexpr ComponentID MaskOperation(ComponentID id, uint16_t mask = MAIN_MASK) 
{
	return static_cast<ComponentID>(static_cast<int>(id) & mask);
}
