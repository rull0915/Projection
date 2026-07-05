//====================================================//
// ファイル名   : RectTransform.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/11
//
// 概要 : UIのトランスフォームコンポーネント
//
// 更新履歴 :
// 2026/05/11 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/UI/UIComponentBase.h"

//====================================================//
// クラス宣言
//====================================================//
class RectTransform : public UIComponentBase
{
private:

	//-----------------------------------------------------
	// メンバ変数
	//-----------------------------------------------------

	// 親
	RectTransform* m_pParent;

	std::vector<RectTransform*> m_children;  // 子のRectTransformのリスト

	// アンカーの位置
	DirectX::SimpleMath::Vector2 m_anchor;  // 親のRectTransformのどこを基準とするか

	// アンカー基準の位置
	DirectX::SimpleMath::Vector2 m_anchoredPosition;    // アンカー基準の相対座標

	// サイズ
	DirectX::SimpleMath::Vector2 m_size;    // width, height
		
	// ピボット
	DirectX::SimpleMath::Vector2 m_pivot;   // anchoredPositionに自分のどこを配置するか

	// スケール
	DirectX::SimpleMath::Vector2 m_scale;

	// 角度
	float m_rotation;

	// ピボットのピクセル座標
	mutable DirectX::SimpleMath::Vector2 m_pivotPixelPos;

	// アンカーのピクセル座標
	mutable DirectX::SimpleMath::Vector2 m_anchorPixelPos;

	// 左上のピクセル座標
	mutable DirectX::SimpleMath::Vector2 m_luPixelPos;
	
	// キャッシュの更新フラグ
	mutable bool m_isDirty;

public:

	//-----------------------------------------------------
	// コンストラクタ / デストラクタ
	//-----------------------------------------------------
	RectTransform(IComponentOwner* pOwner)
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
		ADD_PROPERTY(m_anchoredPosition);
		ADD_PROPERTY(m_anchor);
		ADD_PROPERTY(m_pivot);
		ADD_PROPERTY(m_size);
		ADD_PROPERTY(m_rotation);
		ADD_PROPERTY(m_scale);
	};

	~RectTransform()
	{
		// 親がいれば親から自分を削除
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}

		// 子供を解放
		RemoveChildren();
	}

	//-----------------------------------------------------
	// 公開関数
	//-----------------------------------------------------

	void UpdateCache() const;

	bool Contains(const DirectX::SimpleMath::Vector2& target);

	//-----------------------------------------------------
	// ゲッター
	//-----------------------------------------------------

	// ID取得
	unsigned int GetID() override
	{
		return TypeIDGenerator::GetID<RectTransform>();
	}

	// 親のポインタ
	RectTransform* GetParent() { return m_pParent; }

	// ピボット
	DirectX::SimpleMath::Vector2 GetPivot() const { return m_pivot; }

	// 子供のリスト
	std::vector<RectTransform*>& GetChildren() { return m_children; }    // 子のRectTransformのリストを取得

	// 右方向
	DirectX::SimpleMath::Vector2 GetRight() const { return { std::cosf(GetWorldRotation()), std::sinf(GetWorldRotation()) }; }

	// 上方向
	DirectX::SimpleMath::Vector2 GetUp() const { return { -std::sinf(GetWorldRotation()), std::cosf(GetWorldRotation()) }; }

	// サイズ
	DirectX::SimpleMath::Vector2 GetSize() const { return m_size; }

	// ワールド座標系でのサイズ
	DirectX::SimpleMath::Vector2 GetWorldScale() const 
	{
		return m_scale * (m_pParent ? m_pParent->GetWorldScale() : DirectX::SimpleMath::Vector2(1.0f, 1.0f)); 
	}

	// ワールド座標系での回転
	float GetWorldRotation() const
	{
		return m_rotation + (m_pParent ? m_pParent->GetWorldRotation() : 0.0f);
	}

	// ピボットのピクセル座標
	DirectX::SimpleMath::Vector2 GetPivotPixelPos() const
	{
		if (m_isDirty) UpdateCache();
		return m_pivotPixelPos;
	}

	// アンカーのピクセル座標
	DirectX::SimpleMath::Vector2 GetAnchorPixelPos() const
	{
		if (m_isDirty) UpdateCache();
		return m_pivotPixelPos;
	}

	// 左上のピクセル座標
	DirectX::SimpleMath::Vector2 GetLUPixelPos() const
	{
		if (m_isDirty) UpdateCache();
		return m_luPixelPos;
	}

	//-----------------------------------------------------
	// セッター
	//-----------------------------------------------------

	// 親を設定する関数
	void SetParent(RectTransform* parent);

	// 子供を追加する関数
	void AddChild(RectTransform* child) { // 子のRectTransformを追加
		m_children.push_back(child);
	}

	// 子供を削除する関数
	void RemoveChild(RectTransform* child)
	{
		// リストからchildを探して削除
		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it != m_children.end())
		{
			m_children.erase(it);
		}
	}

	// 全ての子供を削除する関数
	void RemoveChildren() 
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

	// アンカー
	void SetAnchor(DirectX::SimpleMath::Vector2 anchor) {
		m_anchor = anchor;
		SetDirty();
	}

	// アンカー基準の座標
	void SetAnchoredPosition(DirectX::SimpleMath::Vector2 anchoredPosition) {
		m_anchoredPosition = anchoredPosition;
		SetDirty();
	}

	// サイズ
	void SetSize(DirectX::SimpleMath::Vector2 size) {   // サイズ
		m_size = size;
		SetDirty();
	}

	// ピボット
	void SetPivot(DirectX::SimpleMath::Vector2 pivot) { // (0,0)なら左下、(1,1)なら右上
		m_pivot = pivot;
		SetDirty();
	}

	// スケール
	void SetScale(DirectX::SimpleMath::Vector2 scale) { // スケール
		m_scale = scale;
		SetDirty();
	}

	// 回転
	void SetRotation(float rotation) {  // 角度
		m_rotation = rotation;
		SetDirty();
	}

	void SetDirty() 
	{
		m_isDirty = true; 

		// 子のDirtyをtrueに
		for (auto child : m_children)
		{
			child->SetDirty();
		}
	}
private:

	//-----------------------------------------------------
	// 内部実装
	//-----------------------------------------------------

	// 指定したトランスフォームが自分のツリーに含まれるか調べる関数
	bool IsDescendantOf(RectTransform* target)
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
};
