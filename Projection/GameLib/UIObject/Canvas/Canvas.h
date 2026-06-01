//====================================================//
// ファイル名   : Canvas.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/23
//
// 概要 : キャンバスクラス
//
// 更新履歴 :
// 2026/05/23 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "../UIObject.h"
#include "../UIComponents/Graphics/BaseGraphic.h"
#include "GameLib/Input/MouseInput.h"

//====================================================//
// 前方宣言
//====================================================//
class UIManager;

//====================================================//
// クラス宣言
//====================================================//
class Canvas : public UIObject
{
private:

    //-----------------------------------------------------
    // 定数
    //-----------------------------------------------------


    //-----------------------------------------------------
    // メンバ変数
    //-----------------------------------------------------

    // 描画順
	int m_drawOrder;

    // 所有するUIManagerへのポインタ
	UIManager* m_pUIManager;   

    // 生成予約中のオブジェクトリスト
    std::vector<std::unique_ptr<UIObject>> m_reservations;

	// 保持するUIObjectのリスト
	std::vector<std::unique_ptr<UIObject>> m_pUIObjects;

public:

    //-----------------------------------------------------
    // コンストラクタ / デストラクタ
    //-----------------------------------------------------
    Canvas(UIManager* uiManager);
    ~Canvas() = default;

    //-----------------------------------------------------
    // 公開関数
    //-----------------------------------------------------

    void Update(float elapsedTime);

    void Draw(Renderer& renderer);

    // 生成関数
	template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<UIObject, T>::value>>
    T* Generate(Args&&... args);

    // 削除関数
	void Remove(UIObject* obj);

    // 点と衝突するかどうか
    RectTransform* HitTest(const DirectX::SimpleMath::Vector2& point);

    //-----------------------------------------------------
    // ゲッター
    //-----------------------------------------------------

	int GetDrawOrder() const { return m_drawOrder; }

    //-----------------------------------------------------
    // セッター
    //-----------------------------------------------------

    void SetDrawOrder(int order);

private:

    //-----------------------------------------------------
    // 内部実装
    //-----------------------------------------------------
    void UpdateChild(RectTransform* child, float elapsedTime)
    {
        if (!child || !child->GetOwn()) return;

        UIObject* childObj = child->GetOwn();

        if (!childObj->IsActive()) return;

        // 子のUIObjectがアクティブなら
        
        // コンポーネントを登録
        childObj->RegisterComponents();

        // コンポーネントを更新
        childObj->ComponentsUpdate(elapsedTime);

        // 再帰的に更新
        for (auto grandChild : child->GetChildren())
        {
            UpdateChild(grandChild, elapsedTime);
        }

        // コンポーネントを削除
        childObj->RemoveRegistered();
    }
    void DrawChild(RectTransform* child, Renderer& renderer);

    RectTransform* HitTestChild(RectTransform* child, const DirectX::SimpleMath::Vector2& point)
    {
        UIObject* childObj = child->GetOwn();

        if (!childObj->IsActive()) return nullptr;

        // 子のUIObjectがアクティブなら

        // 逆順で再帰的にチェック
        std::vector<RectTransform*>& children = child->GetChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it)
        {
            if (auto* hit = HitTestChild(*it, point))
            {
                return hit;
            }
        }

        // 子をチェックした後に自分をチェック

        // 自分にImageがあるなら
        if (auto* image = childObj->GetComponent<ImageUI>())
        {
            // raycastと当たる設定なら
            if (!image->IsActive() || !image->IsRaycastTarget()) return nullptr;

            // 点が自分の上にあれば
            if (child->Contains(point))
            {
                // 自分を返す
                return child;
            }
        }

        return nullptr;
    }

	// 予約リストから生成予約されたオブジェクトを登録する関数
    void RegisterReservations()
    {
		// 予約リストにあるオブジェクトを全て登録
        for (auto& reservation : m_reservations)
        {
            m_pUIObjects.push_back(std::move(reservation));
        }
        m_reservations.clear();
    }
};

template<typename T, typename ...Args, typename>
inline T* Canvas::Generate(Args && ...args)
{
	std::unique_ptr<T> obj = std::make_unique<T>(std::forward<Args>(args)...);

    // 親を自分に設定
	obj->GetComponent<RectTransform>()->SetParent(GetComponent<RectTransform>());

    // 予約リストに追加
	T* ptr = obj.get();
	m_reservations.push_back(std::move(obj));

    return ptr;
}
