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
#include "Components/UI/RectTransform/RectTransform.h"

#include "System/WindowManager.h"

using namespace DirectX;

namespace RHEngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	RectTransform::RectTransform(IComponentOwner* pOwner)
		: UIComponentBase(pOwner)
		, m_pParent{ nullptr }
		, m_anchor{ 0.5f, 0.5f }
		, m_anchoredPosition{ 0, 0 }
		, m_size{ 100, 100 }
		, m_pivot{ 0.5f, 0.5f }
		, m_scale{ 1.0f, 1.0f }
		, m_rotation{ 0 }
		, m_isDirty{ true }
	{
		ADD_PROPERTY(RectTransform, m_anchoredPosition);
		ADD_PROPERTY(RectTransform, m_anchor);
		ADD_PROPERTY(RectTransform, m_pivot);
		ADD_PROPERTY(RectTransform, m_size);
		ADD_PROPERTY(RectTransform, m_rotation);
		ADD_PROPERTY(RectTransform, m_scale);
	};

	RectTransform::~RectTransform()
	{
		// 親がいれば親から自分を削除
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}

		// 子供を解放
		RemoveChildren();
	}

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
				parentUp * (m_pParent->m_size.y * m_anchor.y * parentScale.y);

			// ピボットの位置を求める
			m_pivotPixelPos = m_anchorPixelPos +
				parentRight * (m_anchoredPosition.x * parentScale.x) +
				parentUp * (m_anchoredPosition.y * parentScale.y);

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
			SimpleMath::Vector2 screenSize = WindowManager::Instance().GetSize();

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

	void RectTransform::AddChild(RectTransform* child)
	{ 
		m_children.push_back(child);
	}

	void RectTransform::RemoveChild(RectTransform* child)
	{
		// リストからchildを探して削除
		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it != m_children.end())
		{
			m_children.erase(it);
		}
	}

	void RectTransform::RemoveChildren()
	{
		// 現在の子リストをコピーする
		auto tempChildren = m_children;

		// コピーしたリストを使って処理
		for (auto* child : tempChildren)
		{
			child->SetParent(nullptr);
		}
		m_children.clear();
	}
	
	void RectTransform::SetDirty()
	{
		m_isDirty = true;

		// 子のDirtyをtrueに
		for (auto child : m_children)
		{
			child->SetDirty();
		}
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
				newParentUp * (parent->m_size.y * m_anchor.y * newParentScale.y);

			// 変換後のアンカーからピボットまでのベクトルを求める
			SimpleMath::Vector2 anchorToPivot = m_pivotPixelPos - newAnchorPixelPos;

			// 新しいアンカー基準の位置を求める
			float parentRot = parent->GetWorldRotation();

			// 各軸に投影
			newAnchoredPosition = SimpleMath::Vector2(
				anchorToPivot.Dot(SimpleMath::Vector2(std::cos(parentRot), std::sin(parentRot))),
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

			m_anchoredPosition = m_pivotPixelPos - WindowManager::Instance().GetSize() * m_anchor;

			if (m_pParent)
			{
				m_pParent->RemoveChild(this);
			}
			m_pParent = nullptr;
		}

		// 変化フラグを立てる
		SetDirty();
	}

	bool RectTransform::IsDescendantOf(RectTransform* target)
	{
		RectTransform* current = target;

		// currentがnullになるまでループ
		while (current != nullptr)
		{
			// 自分と同じかチェック
			if (current == this)
			{
				return true;
			}

			// 親を辿る
			current = current->m_pParent;
		}

		// 最後まで到達した時
		return false;
	}
}	// namespace RHEngine
