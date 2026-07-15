//====================================================//
// ファイル名   : SphereCollider.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/18
//
// 概要 : 球状のコライダー
//
// 更新履歴 :
// 2026/03/18 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../ColliderBase.h"
#include "Components/Interface/IDebugRenderable.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class SphereCollider : public ColliderBase, public IDebugRenderable
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 半径
		float m_radius;

		mutable float m_worldRadius;

	public:

		//-----------------------------------------------------
		// 生成 / 破棄
		//-----------------------------------------------------
		SphereCollider(IComponentOwner* own)
			: ColliderBase(own, ColliderType::Sphere)
			, m_radius{ 0.5f }
			, m_worldRadius{ 1.0f }
		{
			ADD_PROPERTY(m_radius);
		};
		~SphereCollider() = default;

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// キャッシュ更新
		void UpdateCache() const override;

		// デバッグ描画
		void DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color) override;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		// ID取得
		unsigned int GetID() override
		{
			return TypeIDGenerator::GetID<SphereCollider>();
		}

		float GetRadius() const
		{
			if (IsDirty()) UpdateCache();

			return m_worldRadius;
		}

		float GetLocalRadius() const
		{
			return m_radius;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------
		void SetRadius(float rad)
		{
			m_radius = rad;
			SetDirty();
		}
	};
} // namespace REngine
