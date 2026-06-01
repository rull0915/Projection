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
#include "../UIComponent.h"

//====================================================//
// クラス宣言
//====================================================//
class RectTransform : public UIComponent<RectTransform, UIComponentID::RectTransform>
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


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
    RectTransform(UIObject* pOwner)
        : UIComponent(pOwner)
		, m_pParent{ nullptr }
		, m_anchor{ 0.5f, 0.5f }
		, m_anchoredPosition{ 0, 0 }
		, m_size{ 100, 100 }
		, m_pivot{ 0.5f, 0.5f }
		, m_scale{ 1.0f, 1.0f }
		, m_rotation{ 0 }
		, m_isDirty{ true }
    {};
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

    void DebugRender(Renderer& renderer);

    bool Contains(const DirectX::SimpleMath::Vector2& target);

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

    RectTransform* GetParent() { return m_pParent; }

	DirectX::SimpleMath::Vector2 GetPivot() const { return m_pivot; }

	std::vector<RectTransform*>& GetChildren() { return m_children; }    // 子のRectTransformのリストを取得

    DirectX::SimpleMath::Vector2 GetRight() const { return { std::cosf(GetWorldRotation()), std::sinf(GetWorldRotation()) }; }

    DirectX::SimpleMath::Vector2 GetUp() const { return { -std::sinf(GetWorldRotation()), std::cosf(GetWorldRotation()) }; }

    DirectX::SimpleMath::Vector2 GetSize() const { return m_size; }

    DirectX::SimpleMath::Vector2 GetWorldScale() const 
    {
        return m_scale * (m_pParent ? m_pParent->GetWorldScale() : DirectX::SimpleMath::Vector2(1.0f, 1.0f)); 
    }
	float GetWorldRotation() const
	{
		return m_rotation + (m_pParent ? m_pParent->GetWorldRotation() : 0.0f);
	}
    DirectX::SimpleMath::Vector2 GetPivotPixelPos() const
    {
        if (m_isDirty) UpdateCache();
        return m_pivotPixelPos;
    }
    DirectX::SimpleMath::Vector2 GetAnchorPixelPos() const
    {
        if (m_isDirty) UpdateCache();
        return m_pivotPixelPos;
    }
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

	void AddChild(RectTransform* child) { // 子のRectTransformを追加
		m_children.push_back(child);
	}
    void RemoveChild(RectTransform* child)
    {
		// リストからchildを探して削除
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
        {
            m_children.erase(it);
        }
    }
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

	void SetAnchor(DirectX::SimpleMath::Vector2 anchor) {   // (0,0)なら親の左下、(1,1)なら親の右上
        m_anchor = anchor;
        SetDirty();
    }
	void SetAnchoredPosition(DirectX::SimpleMath::Vector2 anchoredPosition) {   // アンカー基準の相対座標
		m_anchoredPosition = anchoredPosition;
		SetDirty();
	}
	void SetSize(DirectX::SimpleMath::Vector2 size) {   // サイズ
        m_size = size;
		SetDirty();
    }
	void SetPivot(DirectX::SimpleMath::Vector2 pivot) { // (0,0)なら左下、(1,1)なら右上
        m_pivot = pivot;
        SetDirty();
	}
	void SetScale(DirectX::SimpleMath::Vector2 scale) { // スケール
        m_scale = scale;
        SetDirty();
    }
	void SetRotation(float rotation) {  // 角度
		m_rotation = rotation;
		SetDirty();
	}

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------
	void SetDirty() 
    {
        m_isDirty = true; 

        // 子のDirtyをtrueに
        for (auto child : m_children)
        {
            child->SetDirty();
        }
    }

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
