//====================================================//
// ファイル名   : BoxCollider2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/04/17
//
// 概要 : 2Dボックスコライダー
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
	class BoxCollider2D : public ColliderBase2D, public IDebugRenderable
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// ボックスのローカルでのサイズ
		DirectX::SimpleMath::Vector2 m_localSize;

		// 計算済みワールド情報
		struct WorldCache {
			DirectX::SimpleMath::Vector2 xAxis;
			DirectX::SimpleMath::Vector2 yAxis;
		};

		mutable WorldCache m_cache;


	public:

		//-----------------------------------------------------
		// 生成 / 破棄
		//-----------------------------------------------------
		BoxCollider2D(IComponentOwner* own)
			: ColliderBase2D(own)
			, m_localSize{ 1, 1 }
			, m_cache{}
		{
			ADD_PROPERTY(BoxCollider2D, m_localSize);
		};
		~BoxCollider2D() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(BoxCollider2D, ColliderBase2D)

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

		// 各軸ベクトルを取得する関数
		float GetAngle() const
		{
			return GetRotation();
		}
		DirectX::SimpleMath::Vector2 GetXAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.xAxis;
		}
		DirectX::SimpleMath::Vector2 GetYAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.yAxis;
		}
		DirectX::SimpleMath::Vector2 GetSize() const
		{
			return m_localSize;
		}
		DirectX::SimpleMath::Vector2 GetHalfSize() const
		{
			return m_localSize * 0.5f;
		}

		DirectX::SimpleMath::Vector2 GetLocalSize() const
		{
			return m_localSize;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetLocalSize(DirectX::SimpleMath::Vector2 scale)
		{
			m_localSize = scale;
			SetDirty();
		}
	};
} // namespace REngine
