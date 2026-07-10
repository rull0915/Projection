//====================================================//
// ファイル名   : ConvexPolygonCollider2D.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/01
//
// 概要 : 2次元凸多角形コライダー
//
// 更新履歴 :
// 2026/06/01 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <vector>
#include <SimpleMath.h>

#include "Components/World/Collider/2D/ColliderBase2D.h"
#include "Components/Interface/IDebugRenderable.h"

//====================================================//
// クラス宣言
//====================================================//
class ConvexPolygonCollider2D : public ColliderBase2D, public IDebugRenderable
{
private:

	//-----------------------------------------------------
	// 定数
	//-----------------------------------------------------


	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 頂点座標のリスト(中心からの相対座標)
	std::vector<DirectX::SimpleMath::Vector2> m_localVertices;

	// ワールド座標系での頂点座標のキャッシュ
	mutable std::vector<DirectX::SimpleMath::Vector2> m_worldVertices;

	// 3D空間での頂点座標のキャッシュ
	mutable std::vector<DirectX::SimpleMath::Vector3> m_worldVertices3D;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	ConvexPolygonCollider2D(IComponentOwner* owner);
	~ConvexPolygonCollider2D() {};

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	// 時計回りに補正する関数
	void CorrectionClockWise();

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
		return TypeIDGenerator::GetID<ConvexPolygonCollider2D>();
	}
	
	const std::vector<DirectX::SimpleMath::Vector2>& GetWorldVertices() const
	{
		if (IsDirty()) UpdateCache();
		return m_worldVertices;
	}

	const std::vector<DirectX::SimpleMath::Vector3>& GetWorldVertices3D() const
	{
		if (IsDirty()) UpdateCache();
		return m_worldVertices3D;
	}
	
	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	// 頂点を追加する関数
	void AddVertex(const DirectX::SimpleMath::Vector2& vertex)
	{
		// 既にその点が存在していれば追加しない
		if (IsContain(vertex)) return;

		m_localVertices.push_back(vertex);

		// もし追加されることで凸でなくなる場合削除する
		if (!IsConvex())
		{
			m_localVertices.pop_back();
		}

		SetDirty();
	}
	void PopVertex()
	{
		m_localVertices.pop_back();
		SetDirty();
	}
	void ResetVertices()
	{
		m_localVertices.clear();
		SetDirty();
	}
	void SetVertices(const std::vector<DirectX::SimpleMath::Vector2>& vertices)
	{
		m_localVertices = vertices;

		// 指定された配列が凸でない場合
		if (!IsConvex())
		{
			// リセットする
			ResetVertices();
		}

		SetDirty();
	}

private: 
	// 凸法であるか調べる関数
	bool IsConvex() const;

	// 既に同じ点が含まれているか調べる関数
	bool IsContain(const DirectX::SimpleMath::Vector2& point);
};
