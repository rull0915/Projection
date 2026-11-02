/**
 * @file    ConvexPolygonCollider2D.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/01
 *
 * @brief   2次元凸多角形コライダー
 */

#pragma once

#include <vector>
#include <SimpleMath.h>

#include "Components/World/Collider/2D/ColliderBase2D.h"
#include "Components/Interface/IDebugRenderable.h"

/**
 * @brief 2D凸多角形コライダー
 */
class ConvexPolygonCollider2D : public RHEngine::ColliderBase2D, public RHEngine::IDebugRenderable
{
public:
	DECLARE_COMPONENT_TYPE(ColliderBase2D);

private:

	/** @brief 頂点座標のリスト(中心からの相対座標) */
	std::vector<DirectX::SimpleMath::Vector2> m_localVertices;

	/** @brief ワールド座標系での頂点座標のキャッシュ */
	mutable std::vector<DirectX::SimpleMath::Vector2> m_worldVertices;

	/** @brief 3D空間での頂点座標のキャッシュ */
	mutable std::vector<DirectX::SimpleMath::Vector3> m_worldVertices3D;

public:

	ConvexPolygonCollider2D(RHEngine::IComponentOwner* owner);
	~ConvexPolygonCollider2D() {};

	/**
	 * @brief 頂点の並びを時計回りに補正する
	 */
	void CorrectionClockWise();

	/**
	 * @brief キャッシュを更新する
	 */
	void UpdateCache() const override;

	/**
	 * @brief 衝突形状のデバッグ描画
	 */
	void DebugRender(RHEngine::Renderer& renderer, const DirectX::SimpleMath::Color& color) override;

	/**
	 * @brief 2Dワールド座標系での頂点座標リストを取得する
	 */
	const std::vector<DirectX::SimpleMath::Vector2>& GetWorldVertices() const
	{
		if (IsDirty()) UpdateCache();
		return m_worldVertices;
	}

	/**
	 * @brief 3Dワールド座標系での頂点座標リストを取得する
	 */
	const std::vector<DirectX::SimpleMath::Vector3>& GetWorldVertices3D() const
	{
		if (IsDirty()) UpdateCache();
		return m_worldVertices3D;
	}


	/**
	 * @brief 凸多角形の頂点を追加する
	 * @param vertex 追加する頂点
	 */
	void AddVertex(const DirectX::SimpleMath::Vector2& vertex)
	{
		// 既にその点が存在していれば追加しない
		if (IsContain(vertex)) return;

		m_localVertices.push_back(vertex);

		SetDirty();
	}

	/**
	 * @brief 最後に追加した頂点を削除する
	 */
	void PopVertex()
	{
		m_localVertices.pop_back();
		SetDirty();
	}

	/**
	 * @brief 形状をリセットする
	 */
	void ResetVertices()
	{
		m_localVertices.clear();
		SetDirty();
	}

	/**
	 * @brief 凸多角形をそのままセットする
	 * @param vertices 
	 */
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
	
	/**
	 * @brief 現在の形状が凸包かどうか調べる
	 */
	bool IsConvex() const;

	/**
	 * @brief 既に指定した点が含まれているか調べる
	 * @param point 調べる点
	 */
	bool IsContain(const DirectX::SimpleMath::Vector2& point);
};
