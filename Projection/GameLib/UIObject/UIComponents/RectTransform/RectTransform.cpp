//====================================================//
// ファイル名  : RectTransform.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/14
//
// 概要        :  UI用トランスフォーム
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "RectTransform.h"

#include "GameLib/Common/Renderer/Renderer.h"

using namespace DirectX;

//====================================================//
// 関数の実体宣言
//====================================================//

void RectTransform::UpdateCache() const
{
	// 親がいる場合
	if (m_pParent)
	{
		if (m_pParent->m_isDirty) m_pParent->UpdateCache();

		// 親のスケールを取得
		SimpleMath::Vector2 parentScale = m_pParent->GetWorldScale();

		// アンカーのピクセル座標を求める

		// 0 ~ 1 の値を -1 ~ +1 に変換

		// 親の回転
		SimpleMath::Vector2 parentRight = m_pParent->GetRight();
		SimpleMath::Vector2 parentUp = m_pParent->GetUp();

		// アンカーの座標を求める
		m_anchorPixelPos = m_pParent->m_luPixelPos +
			parentRight * (m_pParent->m_size.x * m_anchor.x * parentScale.x) +
			parentUp	* (m_pParent->m_size.y * m_anchor.y * parentScale.y);

		// ピボットの位置を求める
		m_pivotPixelPos = m_anchorPixelPos + 
			parentRight * (m_anchoredPosition.x * parentScale.x) + 
			parentUp	* (m_anchoredPosition.y * parentScale.y);

		// サイズ 回転 スケールを考慮して左上座標を求める

		// サイズとスケールを掛け矩形のサイズを求める
		SimpleMath::Vector2 worldSize = { m_size.x * parentScale.x * m_scale.x, m_size.y * parentScale.y * m_scale.y };

		// ピボットの位置から矩形の左上までの距離を求める
		SimpleMath::Vector2 distance{
			worldSize.x * (-m_pivot.x) ,
			worldSize.y * (-m_pivot.y) };

		// 回転を考慮して左上座標を求める
		m_luPixelPos = {
			m_pivotPixelPos + 
			distance.x * GetRight() +
			distance.y * GetUp()
		};
	}
	// 親がいない場合 (Canvasの場合)
	else
	{
		// スクリーンのサイズを取得
		SimpleMath::Vector2 screenSize = { static_cast<float>(Screen::WIDTH), static_cast<float>(Screen::HEIGHT) };

		// アンカーのピクセル座標を求める
		SimpleMath::Vector2 anchorPos = 
		{
			screenSize.x * m_anchor.x, 
			screenSize.y * m_anchor.y 
		};

		// ピボットの位置を求める
		m_pivotPixelPos = anchorPos + m_anchoredPosition;

		// サイズ 回転 スケールを考慮して中心座標を求める

		// サイズとスケールを掛け矩形のサイズを求める
		SimpleMath::Vector2 worldSize = { m_size.x * m_scale.x, m_size.y * m_scale.y };

		// ピボットの位置から左上までの距離を求める
		SimpleMath::Vector2 distance = { worldSize.x * (-m_pivot.x), worldSize.y * (-m_pivot.y) };

		// 回転を考慮して左上座標を求める
		m_luPixelPos = {
			m_pivotPixelPos + 
			distance.x * GetRight() +
			distance.y * GetUp()
		};
	}

	m_isDirty = false;
}

/// <summary>
/// デバッグ描画関数
/// </summary>
void RectTransform::DebugRender(Renderer& renderer)
{
	if (m_isDirty) UpdateCache();

	SimpleMath::Vector2 ws = GetWorldScale();

	SimpleMath::Vector2 points[4] =
	{
		m_luPixelPos,
		m_luPixelPos + GetRight() * m_size.x * ws.x,
		m_luPixelPos + GetRight() * m_size.x * ws.x + GetUp() * m_size.y * ws.y,
		m_luPixelPos + GetUp() * m_size.y * ws.y,
	};

	for (int i = 0; i < 4; i++)
	{
		renderer.Draw().UI().Line(points[i], points[(i + 1) % 4], 0x00FFFF);
	}
}

bool RectTransform::Contains(const SimpleMath::Vector2& target)
{
    // pivot基準のワールド位置
    SimpleMath::Vector2 pivotPos = GetPivotPixelPos();

    // 点→pivot
    SimpleMath::Vector2 diff = target - pivotPos;

    //--------------------------------
    // 回転を打ち消す
    //--------------------------------

    SimpleMath::Vector2 local =
    {
        diff.Dot(GetRight()),
        diff.Dot(GetUp()),
    };

    //--------------------------------
    // scaleを打ち消す
    //--------------------------------

    SimpleMath::Vector2 scale = GetWorldScale();

    local.x /= scale.x;
    local.y /= scale.y;

    //--------------------------------
    // pivot基準の矩形判定
    //--------------------------------

    SimpleMath::Vector2 size = GetSize();

    SimpleMath::Vector2 min =
    {
        -size.x * GetPivot().x,
        -size.y * GetPivot().y
    };

    SimpleMath::Vector2 max =
    {
        size.x * (1.0f - GetPivot().x),
        size.y * (1.0f - GetPivot().y)
    };

    return
        (local.x >= min.x &&
        local.x <= max.x) &&
        (local.y >= min.y &&
        local.y <= max.y);
}

void RectTransform::SetParent(RectTransform* parent)
{
	// 自分か自分の子孫を親にしようとしていたら
	if (parent == this || IsDescendantOf(parent)) return;

	// 親が変わらないなら何もしない
	if (m_pParent == parent) return;

	// 指定された親がnullでない場合
	if (parent)
	{
		if (parent->m_isDirty) parent->UpdateCache();
		
		// ワールドでの座標が変化しないように各パラメータを算出

		float newRotation = GetWorldRotation() - parent->GetWorldRotation();	// 回転は親の回転を引く
		SimpleMath::Vector2 newScale = GetWorldScale() / parent->GetWorldScale();	// スケールは親のスケールで割る

		SimpleMath::Vector2 newAnchoredPosition;

		// 変換後のアンカーのピクセル座標を求める

		// 新しい親のスケールを取得
		SimpleMath::Vector2 newParentScale = parent->GetWorldScale();

		// 新しい親の回転
		SimpleMath::Vector2 newParentRight = parent->GetRight();
		SimpleMath::Vector2 newParentUp = parent->GetUp();

		// アンカーの座標を求める 
		SimpleMath::Vector2 newAnchorPixelPos = parent->m_luPixelPos +
			newParentRight * (parent->m_size.x * m_anchor.x * newParentScale.x) +
			newParentUp	* (parent->m_size.y * m_anchor.y * newParentScale.y);

		// 変換後のアンカーからピボットまでのベクトルを求める
		SimpleMath::Vector2 anchorToPivot = m_pivotPixelPos - newAnchorPixelPos;

		// 新しいアンカー基準の位置を求める
		float parentRot = parent->GetWorldRotation();

		// 各軸に投影
		newAnchoredPosition = SimpleMath::Vector2(
			anchorToPivot.Dot(SimpleMath::Vector2( std::cos(parentRot), std::sin(parentRot))),		
			anchorToPivot.Dot(SimpleMath::Vector2(-std::sin(parentRot), std::cos(parentRot)))
		) / newParentScale;

		// 別の親に所属していたら
		if (m_pParent) m_pParent->RemoveChild(this);    // 親から自分を削除

		// 新しい親を設定
		m_pParent = parent;

		// 新しい親に自分を追加
		m_pParent->AddChild(this);

		// パラメータを更新
		m_rotation = newRotation;
		m_scale = newScale;
		m_anchoredPosition = newAnchoredPosition;
	}
	else
	{
		m_rotation = GetWorldRotation();
		m_scale = GetWorldScale();

		m_anchoredPosition = m_pivotPixelPos - SimpleMath::Vector2(Screen::WIDTH * m_anchor.x, Screen::HEIGHT * m_anchor.y);

		if (m_pParent)
		{
		    m_pParent->RemoveChild(this);
		}
		m_pParent = nullptr;
	}

    // 変化フラグを立てる
    SetDirty();
}
