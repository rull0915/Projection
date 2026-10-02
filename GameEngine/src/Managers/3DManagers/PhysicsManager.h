//====================================================//
// ファイル名   : PhysicsManager.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/08
//
// 概要 : 物理挙動管理クラスです
//
// 更新履歴 :
// 2026/04/08 新規作成
// 2026/04/22 衝突管理クラスを呼び出すように。
// 2026/05/04 シングルトンから通常のクラスへ変更
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/RigidBody/RigidBody.h"
#include "Physics/HitContact.h"
#include "../ReserveContainer.h"

#include <unordered_map>

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//

	class CollideManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class PhysicsManager : public ReserveContainer<RigidBody>
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 衝突管理クラス
		std::unique_ptr<CollideManager> m_collideManager;

		// 衝突情報
		std::vector<HitContact> m_contacts;
		std::unordered_map<ObjectPair, HitContact, ObjectPairHash> m_contactMap;

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
	public:
		PhysicsManager();
		~PhysicsManager();
		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// 更新処理
		void Update(float elapsedTime);

		void ReflectReserves();

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------
		std::unordered_map<ObjectPair, HitContact, ObjectPairHash>& GetHitList() { return m_contactMap; }

		CollideManager* GetCollideManager() { return m_collideManager.get(); }

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

	private:
		// 衝突後の補正
		void HittedCorrection();

		void PositionCorrection(HitContact& contact);
		void VelocityCorrection(HitContact& contact);
	};
}	// namespace REngine
