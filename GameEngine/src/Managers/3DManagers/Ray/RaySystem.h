//====================================================//
// ファイル名   : RaySystem.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/26
//
// 概要 : Rayのシステムを管理するクラス
//
// 更新履歴 :
// 2026/06/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Physics/RaycastHit.h"
#include "Physics/Ray.h"

//====================================================//
// 前方宣言
//====================================================//
class ColliderBase;

//====================================================//
// クラス宣言
//====================================================//

// Ray vs コライダー衝突マップ
class RaySystem
{
	// マクロ宣言
	using CollisionFunc = std::function<bool(const Ray&, float, ColliderBase*, RaycastHit*)>;

private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 型IDをキーとした衝突判定関数の2次元マップ
	inline static std::unordered_map<uint16_t, CollisionFunc> m_collisionMap;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	RaySystem() = default;
	~RaySystem() = default;

	// 関数登録
	static bool Register(uint16_t id, CollisionFunc func);

	// 関数実行
	static bool CheckHit(const Ray&, float, ColliderBase*, RaycastHit*);

	// 判定
	static bool RayCast(const std::vector<ColliderBase*>& colliders, const Ray& ray, float maxDistance, RaycastHit* hit, uint64_t layerMask = 0xFFFFFFFFFFFFFFFF);
};
