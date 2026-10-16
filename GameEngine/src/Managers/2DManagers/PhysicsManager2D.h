//====================================================//
// ファイル名   : PhysicsManager2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/16
//
// 概要 : 2D版物理挙動管理クラスです
//
// 更新履歴 :
// 2026/04/16 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/RigidBody/RigidBody2D.h"
#include "Physics/HitContact.h"
#include "../ReserveContainer.h"

#include <memory>

namespace RHEngine
{
	//====================================================//
	// 前方宣言
	//====================================================//

	class CollideManager2D;

	//====================================================//
	// クラス宣言
	//====================================================//
	class PhysicsManager2D : public ReserveContainer<RigidBody2D>
	{
	private:

		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 衝突管理クラス
		std::unique_ptr<CollideManager2D> m_collideManager;

		// 衝突情報
		std::vector<HitContact2D> m_contacts;
		std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D> m_contactMap;

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
	public:
		PhysicsManager2D();
		~PhysicsManager2D();
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 更新処理
		void Update(float elapsedTime);

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------
		std::unordered_map<ObjectPair2D, HitContact2D, ObjectPairHash2D>& GetHitList() { return m_contactMap; }

		CollideManager2D* GetCollideManager() { return m_collideManager.get(); }

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

	private:
		// 衝突後の補正
		void HittedCorrection();

		void PositionCorrection(HitContact2D& contact);
		void VelocityCorrection(HitContact2D& contact);
	};
}	// namespace RHEngine
