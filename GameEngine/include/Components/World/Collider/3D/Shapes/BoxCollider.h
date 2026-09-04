//====================================================//
// ファイル名   : BoxCollider.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/03/18
//
// 概要 : ボックスのコライダー
//
// 更新履歴 :
// 2026/03/18 新規作成
// 2026/04/03 設計を変更
// 2026/07/10 デバッグ描画をインターフェース化しました
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
	class BoxCollider : public ColliderBase, public IDebugRenderable
	{
	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// ボックスのローカルでのサイズ
		DirectX::SimpleMath::Vector3 m_localSize;

		// 計算済みワールド情報
		struct WorldCache {
			DirectX::SimpleMath::Vector3 xAxis;
			DirectX::SimpleMath::Vector3 yAxis;
			DirectX::SimpleMath::Vector3 zAxis;

			DirectX::SimpleMath::Vector3 scale;

			DirectX::SimpleMath::Matrix localMatrix;
			DirectX::SimpleMath::Matrix localMatrixInverse;
		};

		mutable WorldCache m_cache;

	public:

		//-----------------------------------------------------
		// 生成 / 破棄
		//-----------------------------------------------------
		BoxCollider(IComponentOwner* own)
			: ColliderBase(own, ColliderType::Box)
			, m_localSize{ 1, 1, 1 }
			, m_cache{}
		{
			ADD_PROPERTY(BoxCollider, m_localSize);
		};
		~BoxCollider() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(BoxCollider, ColliderBase)

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		// キャッシュの更新
		void UpdateCache() const override;

		// デバッグ描画
		void DebugRender(Renderer& renderer, const DirectX::SimpleMath::Color& color) override;

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		DirectX::SimpleMath::Vector3 GetLocalSize() const
		{
			return m_localSize;
		}

		// 各軸ベクトルを取得する関数
		DirectX::SimpleMath::Vector3 GetXAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.xAxis;
		}
		DirectX::SimpleMath::Vector3 GetYAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.yAxis;
		}
		DirectX::SimpleMath::Vector3 GetZAxis() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.zAxis;
		}
		DirectX::SimpleMath::Vector3 GetSize() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.scale;
		}
		DirectX::SimpleMath::Vector3 GetHalfSize() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.scale * 0.5f;
		}
		DirectX::SimpleMath::Matrix GetLocalMatrix() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.localMatrix;
		}
		DirectX::SimpleMath::Matrix GetLocalMatrixInverse() const
		{
			if (IsDirty()) UpdateCache();
			return m_cache.localMatrixInverse;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetLocalSize(DirectX::SimpleMath::Vector3 scale)
		{
			m_localSize = scale;
			SetDirty();
		}
	};
} // namespace REngine
