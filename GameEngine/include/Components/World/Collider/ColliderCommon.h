//====================================================//
// ファイル名   : ColliderCommon.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/22
//
// 概要 : コライダーインターフェース
//
// 更新履歴 :
// 2026/04/22 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/World/WorldComponentBase.h"
#include "Components/World/Transform/Transform.h"

#include "GameObject/Interface/IComponentOwner.h"
#include "GameObject/Interface/IColliderReceiver.h"
#include "Components/Interface/IAssetDependent.h"

#include "Assets/Types/PhysicsMaterial.h"
#include "Assets/Objects/Handle.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class Renderer;
	class AssetManager;

	//====================================================//
	// クラス宣言
	//====================================================//
	class ColliderCommon : public WorldComponentBase, public IAssetDependent
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 自身のレイヤー
		int m_layerNum;

		// 親のトランスフォーム
		Transform* m_pTransform;

		// トリガーフラグ
		bool m_isTrigger;

		// 衝突情報を必要とするかのフラグ(トリガーの時)
		bool m_needInfo;

		// 変更済みフラグ
		mutable bool m_isChanged;

		// 値が変更されているかのフラグ
		mutable bool m_isDirty;

		// 最新のバージョン
		mutable uint32_t m_latestVersion;

		// 応答仲介インターフェース
		IColliderReceiver* m_colliderReceiver;

		// 物理マテリアル
		Handle<PhysicsMaterial> m_physicsMaterial;

		// AssetManager
		AssetManager* m_assetManager;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ColliderCommon(IComponentOwner* own);
		virtual ~ColliderCommon() = default;

		// AssetManagerを受け取る関数
		void ReceiveAssetManager(AssetManager& asset) override { m_assetManager = &asset; }

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------
		inline int GetLayer() const { return m_layerNum; }

		inline bool IsTrigger() const { return m_isTrigger; }
		inline bool IsNeedInfo() const { return m_needInfo; }
		inline bool IsChanged() const { return m_isChanged; }
		inline bool IsDirty() const { return m_isDirty || m_pTransform->GetVersion() != m_latestVersion; }

		inline Transform* GetTransform() const { return m_pTransform; }

		const PhysicsMaterial* GetPhysicsMaterial() const;

		inline IColliderReceiver* GetReceiver() const { return m_colliderReceiver; }
		uint32_t GetVersion() const { return m_latestVersion; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetLayer(int num) { m_layerNum = std::min(num, 99); }
		void SetDirty() { m_isDirty = true; };
		void SetTrigger(bool flag) { m_isTrigger = flag; }
		void SetNeedInfo(bool flag) { m_needInfo = flag; }
		void ResetChangeFrag() { m_isChanged = false; }
		void SetPhysicsMaterial(Handle<PhysicsMaterial> mat) { m_physicsMaterial = mat; }

	protected:

		inline void ResetDirty() const { m_isDirty = false; }
		inline void SetChanged(bool flag) const { m_isChanged = flag; }
		inline void ApplyVersion() const { m_latestVersion = m_pTransform->GetVersion(); }
	};
} // namespace REngine
