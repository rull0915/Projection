//====================================================//
// ファイル名   : CollisionMap.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/26
//
// 概要 : 衝突判定のマップをもつクラス
//
// 更新履歴 :
// 2026/06/26 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <functional>
#include <unordered_map>
#include <cstdint>

#include "Physics/HitInfomation.h"
#include "Physics/HitInfomation2D.h"
#include "Components/ComponentBase.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class ColliderBase;
	class ColliderBase2D;

	//====================================================//
	// クラス宣言
	//====================================================//

	// 2つのIDを持ったキー
	struct CollisionKey
	{
		Component::TypeId type1;
		Component::TypeId type2;

		bool operator==(const CollisionKey&) const = default;
	};

	// ハッシュ
	struct CollisionKeyHash
	{
		size_t operator()(const CollisionKey& key) const
		{
			const size_t h1 = std::hash<Component::TypeId>{}(key.type1);
			const size_t h2 = std::hash<Component::TypeId>{}(key.type2);

			return h1 ^ (h2 << 1);
		}
	};

	// 3次元コライダー衝突マップ
	class CollisionMap
	{
		// マクロ宣言
		using CollisionFunc = std::function<bool(ColliderBase*, ColliderBase*, HitInfomation*)>;

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 型IDをキーとした衝突判定関数の2次元マップ
		inline static std::unordered_map<CollisionKey, CollisionFunc, CollisionKeyHash> m_collisionMap;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		CollisionMap() = default;
		~CollisionMap() = default;


		// 関数登録
		static bool Register(Component::TypeId id1, Component::TypeId id2, CollisionFunc func);

		// 関数実行
		static bool CheckHit(ColliderBase* col1, ColliderBase* col2, HitInfomation* info);
	};

	// 2次元コライダー衝突マップ
	class CollisionMap2D
	{
		// マクロ宣言
		using CollisionFunc = std::function<bool(ColliderBase2D*, ColliderBase2D*, HitInfomation2D*)>;

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 型IDをキーとした衝突判定関数の2次元マップ
		inline static std::unordered_map<CollisionKey, CollisionFunc, CollisionKeyHash> m_collisionMap;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		CollisionMap2D() = default;
		~CollisionMap2D() = default;


		// 関数登録
		static bool Register(Component::TypeId id1, Component::TypeId id2, CollisionFunc func);

		// 関数実行
		static bool CheckHit(ColliderBase2D* col1, ColliderBase2D* col2, HitInfomation2D* info);
	};
}	// namespace REngine
