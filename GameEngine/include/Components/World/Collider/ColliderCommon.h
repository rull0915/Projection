/**
 * @file    ColliderCommon.h
 * @author  Hoshino Ryunosuke
 * @date    2026/04/22
 *
 * @brief   コライダー
 */

#pragma once

#include <cstdint>
#include <algorithm>

#include "Components/World/WorldComponentBase.h"
#include "GameObject/Interface/IColliderReceiver.h"
#include "Components/Interface/IAssetDependent.h"
#include "Assets/Types/PhysicsMaterial.h"
#include "Assets/Objects/Handle.h"
#include "Components/World/Transform/Transform.h"

namespace RHEngine
{
	class IComponentOwner;
	class Renderer;
	class AssetManager;

	class ColliderCommon : public WorldComponentBase, public IAssetDependent
	{
	public:
		DECLARE_COMPONENT_TYPE(WorldComponentBase);

	private:
	
		/** @brief 自身のレイヤー番号 */
		int m_layerNum;

		/** @brief 同じオブジェクトのTransform */
		Transform* m_pTransform;

		/** @brief トリガーかどうか */
		bool m_isTrigger;

		/** @brief 衝突情報を調べるかどうか (トリガー時にも衝突法線などを調べるか) */
		bool m_needInfo;

		/** @brief 状態変更フラグ (ツリーの再構築に使用) */
		mutable bool m_isChanged;

		/** @brief 値が変わったかどうか */
		mutable bool m_isDirty;

		/** @brief バージョン番号 */
		mutable uint32_t m_latestVersion;

		/** @brief 衝突応答仲介インターフェース */
		IColliderReceiver* m_colliderReceiver;

		/** @brief 物理マテリアルのハンドル */
		Handle<PhysicsMaterial> m_physicsMaterial;

		/** @brief アセットマネージャーのポインタ */
		AssetManager* m_pAssetManager;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param own 所有者のポインタ
		 */
		ColliderCommon(IComponentOwner* own);

		virtual ~ColliderCommon() = default;

		/// @}

		/**
		 * @brief アセットマネージャーを受け取る
		 */
		void ReceiveAssetManager(AssetManager& asset) override { m_pAssetManager = &asset; }

		/// @name ゲッター
		/// @{

		/** @brief レイヤー番号 */
		inline int GetLayer() const { return m_layerNum; }

		/** @brief トリガーかどうか */
		inline bool IsTrigger() const { return m_isTrigger; }

		/** @brief トリガーの時衝突情報が必要かどうか */
		inline bool IsNeedInfo() const { return m_needInfo; }

		/** @brief 変更されたかどうか */
		inline bool IsChanged() const { return m_isChanged; }

		/** @brief 値が変わっているかどうか */
		inline bool IsDirty() const { return m_isDirty || m_pTransform->GetVersion() != m_latestVersion; }

		/** @brief トランスフォームポインタ */
		inline Transform* GetTransform() const { return m_pTransform; }
		
		/**
		 * @brief 物理マテリアル
		 * @TODO ハンドルを返して使う側でAM経由で本体を取得するように
		 */
		const PhysicsMaterial* GetPhysicsMaterial() const;

		/** @brief 衝突応答仲介インターフェース */
		inline IColliderReceiver* GetReceiver() const { return m_colliderReceiver; }

		/** @brief バージョン番号 */
		uint32_t GetVersion() const { return m_latestVersion; }

		/// @}

		/// @name セッター
		/// @{

		/** @brief レイヤー番号 */
		void SetLayer(int num) { m_layerNum = std::min(num, 99); }

		/** @brief 変更されているかどうか */
		void SetDirty() { m_isDirty = true; };

		/** @brief トリガーかどうか */
		void SetTrigger(bool flag) { m_isTrigger = flag; }

		/** @brief トリガー時に衝突情報が必要かどうか */
		void SetNeedInfo(bool flag) { m_needInfo = flag; }

		/** @brief 変更フラグのリセット */
		void ResetChangeFrag() { m_isChanged = false; }

		/** @brief 物理マテリアル */
		void SetPhysicsMaterial(Handle<PhysicsMaterial> mat) { m_physicsMaterial = mat; }

		/// @}

	protected:
		/**
		 * @brief Dirtyフラグのリセット
		 */
		inline void ResetDirty() const { m_isDirty = false; }

		/**
		 * @brief 変更フラグの変更
		 */
		inline void SetChanged(bool flag) const { m_isChanged = flag; }

		/**
		 * @brief バージョンを最新に更新
		 */
		inline void ApplyVersion() const { m_latestVersion = m_pTransform->GetVersion(); }
	};
} // namespace RHEngine
