//====================================================//
// ファイル名   : CircleCollider2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要 : 2Dの円衝突クラス
//
// 更新履歴 :
// 2026/04/17 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../ColliderBase2D.h"
#include "Components/Interface/IDebugRenderable.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class CircleCollider2D : public ColliderBase2D, public IDebugRenderable
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 半径
		float m_radius;
	public:

		//-----------------------------------------------------
		// 生成 / 破棄
		//-----------------------------------------------------
		CircleCollider2D(IComponentOwner* own)
			: ColliderBase2D(own)
			, m_radius{ 0.5f }
		{
			ADD_PROPERTY(m_radius);
		};
		~CircleCollider2D() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(CircleCollider2D, ColliderBase2D)

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

		float GetRadius() const
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
