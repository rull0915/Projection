//====================================================//
// ファイル名   : ColliderBase.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/18
//
// 概要 : 衝突判定の基底クラス
//
// 更新履歴 :
// 2026/03/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../ColliderCommon.h"
#include "Math/Bounding.h"

namespace REngine
{
	//====================================================//
	// 列挙型宣言
	//====================================================//
	enum class ColliderType
	{
		Sphere,
		Line,
		Capsule,
		Box,
	};

	//====================================================//
	// クラス宣言
	//====================================================//
	class ColliderBase : public ColliderCommon
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// コライダーのタイプ
		const ColliderType m_type;

		// ローカル中心座標
		DirectX::SimpleMath::Vector3 m_localCenterPos;

		// 自身を覆うBoundingBox
		mutable BoundingBox m_boundingBox;

		// ワールド中心座標のキャッシュ
		mutable DirectX::SimpleMath::Vector3 m_worldCenterPos;

	public:

		//-----------------------------------------------------
		// 生成 / 破棄
		//-----------------------------------------------------
		ColliderBase(IComponentOwner* own, ColliderType type)
			: ColliderCommon(own)
			, m_type{ type }
			, m_boundingBox{ {0, 0, 0}, {0, 0, 0} }
			, m_localCenterPos{ 0, 0, 0 }
		{
			ADD_PROPERTY(ColliderBase, m_localCenterPos);
		}

		virtual ~ColliderBase() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(ColliderBase, ColliderCommon)

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// キャッシュの更新をする関数
		virtual void UpdateCache() const = 0;

		// GUI変更時
		void OnValidate() override
		{
			SetDirty();
		}

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		ColliderType GetType() const { return m_type; };

		inline BoundingBox& GetBoundingBox() const { return m_boundingBox; } // 自身を覆うBoundingBoxを取得する関数

		// ワールド座標系での中心座標を返す関数
		DirectX::SimpleMath::Vector3 GetWorldCenterPos() const
		{
			if (IsDirty()) UpdateCache();
			return m_worldCenterPos;
		}
		// ローカル座標系
		DirectX::SimpleMath::Vector3 GetLocalCenterPos() const { return m_localCenterPos; }

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------
		void SetLocalPos(DirectX::SimpleMath::Vector3 pos)
		{
			m_localCenterPos = pos;
			SetDirty();
		}

	protected:

		inline void SetWorldPosition(const DirectX::SimpleMath::Vector3& pos) const { m_worldCenterPos = pos; }
		inline void SetBoundingBox(const BoundingBox& box) const { m_boundingBox = box; }
	};
} // namespace REngine
