//====================================================//
// ファイル名  : Canvas.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/23
//
// 概要       : キャンバスクラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Managers/UI/Canvas.h"

#include "Components/UI/Graphics/UIGraphicBase.h"
#include "Components/UI/Behaviour/UIBehaviorBase.h"
#include "Components/UI/Graphics/ImageUI.h"
#include "Managers/UI/UIManager.h"

#include "System/WindowManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

Canvas::Canvas(UIManager* uiManager)
	: m_isActive{ true }
	, m_canvasName{}
	, m_drawOrder{ 0 }
	, m_pUIManager{ uiManager }
	, m_rootObject{}
	, m_reservations{}
	, m_uiObjects{}
	, m_components{}
{
	ADD_PROPERTY(m_canvasName);
	ADD_PROPERTY(m_drawOrder);

	// シーンを設定
	m_pScene = m_pUIManager->GetScene();

	// ルートオブジェクトを生成
	m_rootObject = std::make_unique<GameObject>(GameObject::CreateToken{});
	m_rootObject->SetScene(m_pScene);

	// ルートRectTransformの初期設定
	auto* rectTransform = m_rootObject->AddComponent<RectTransform>();

	rectTransform->SetAnchor({ 0.5f, 0.5f });
	rectTransform->SetSize({ WindowManager::Instance().GetWidthF(), WindowManager::Instance().GetHeightF() });

	rectTransform->SetAnchoredPosition({ 0, 0 });
	rectTransform->SetPivot({ 0.5f, 0.5f });
}

void Canvas::Update(const GameTimer& gameTimer)
{
	// 生成予約されたオブジェクトを登録
	AddReserves();

	// 全オブジェクトのコンポーネントを追加
	for (auto& object : m_uiObjects)
	{
		// アクティブチェック
		if (!object->IsActive()) continue;

		// 予約されたコンポーネントを追加
		object->GetComponentContainer().RegisterComponents();
	}

	// 全オブジェクトの更新関数呼び出し
	for (auto& object : m_uiObjects)
	{
		// アクティブチェック
		if (!object->IsActive()) continue;

		// 更新処理
		object->BaseUpdate(gameTimer);
	}
}

void Canvas::LateUpdate(const GameTimer& gameTimer)
{
	// 全オブジェクトの遅延更新を呼び出す
	for (auto& object : m_uiObjects)
	{
		// アクティブチェック
		if (!object->IsActive()) continue;

		// 予約されたコンポーネントを追加
		object->BaseLateUpdate(gameTimer);
	}
}

void Canvas::Draw(Renderer& renderer)
{
	// 全ての子に対して操作
	for (auto& child : m_rootObject->GetComponent<RectTransform>()->GetChildren())
	{
		// 子を描画
		DrawChild(child, renderer);
	}
}

void Canvas::RemoveDeadComponent()
{
	// 全オブジェクトの予約済みコンポーネントを削除
	for (auto& object : m_uiObjects)
	{
		// アクティブチェック
		if (!object->IsActive()) continue;

		// 予約されたコンポーネントを追加
		object->GetComponentContainer().RemoveRegistered();
	}
}

void Canvas::Finalize()
{
	AllDestroy();
	RemoveReserves();
}

GameObject* Canvas::Generate()
{
	// ポインタを作成
	GameObject* pObj = new GameObject(GameObject::CreateToken{});

	// オブジェクトのシーンを設定
	pObj->SetScene(m_pScene);

	// 配列に追加
	m_reservations.push_back(std::unique_ptr<GameObject>(pObj));

	// RectTransformを追加
	pObj->AddComponent<RectTransform>();

	// 親をルートに設定
	pObj->GetComponent<RectTransform>()->SetParent(m_rootObject->GetComponent<RectTransform>());

	// 作成したポインタを返す
	return pObj;
}

void Canvas::Remove(GameObject* obj)
{
	// 指定されたオブジェクトを探す
	auto it = std::find_if(m_uiObjects.begin(), m_uiObjects.end(),
		[obj](const std::unique_ptr<GameObject>& ptr) { return ptr.get() == obj; });

	// リストにあれば削除
	if (it != m_uiObjects.end())
	{
		m_uiObjects.erase(it);
	}
}

/// <summary>
/// 指定した点が当たっているRectTransformを返す関数
/// </summary>
RectTransform* Canvas::HitTest(const DirectX::SimpleMath::Vector2& point)
{
	auto& children = m_rootObject->GetComponent<RectTransform>()->GetChildren();

	// 管理しているオブジェクトを走査
	for (auto it = children.rbegin();
		it != children.rend();
		++it)
	{
		// 子の中で当たっているRectを取得
		if (auto* hit = HitTestChild((*it)->GetComponent<RectTransform>(), point))
		{
			// 返す
			return hit;
		}
	}

	return nullptr;
}

void Canvas::OnMouseDown()
{
	// 全ての子に対して操作
	for (auto& child : m_rootObject->GetComponent<RectTransform>()->GetChildren())
	{
		// 子を描画
		MouseCheckChild(child, true);
	}
}

void Canvas::OnMouseUp()
{
	// 全ての子に対して操作
	for (auto& child : m_rootObject->GetComponent<RectTransform>()->GetChildren())
	{
		// 子を描画
		MouseCheckChild(child, false);
	}
}

// 描画順のセッター
void Canvas::SetDrawOrder(int order)
{
	m_drawOrder = order;

	// UIManagerに順番の更新を通知
	m_pUIManager->SetNeedSort(true);
}

void Canvas::DrawChild(RectTransform* child, Renderer& renderer)
{
	GameObject* childObj = static_cast<GameObject*>(child->GetOwn());

	if (!childObj->IsActive()) return;

	// 子のUIObjectがアクティブなら

	// Graphicを継承しているコンポーネントを取得
	std::vector<ComponentBase*> list;
	childObj->GetComponentsWithCategory(Category::UIGraphic, list);

	for (auto graphic : list)
	{
		// 描画
		if (graphic->IsActive()) static_cast<UIGraphicBase*>(graphic)->Draw(renderer);
	}

	// 再帰的に描画
	for (auto grandChild : child->GetChildren())
	{
		DrawChild(grandChild, renderer);
	}
}

RectTransform* Canvas::HitTestChild(RectTransform* child, const DirectX::SimpleMath::Vector2& point)
{
	GameObject* childObj = static_cast<GameObject*>(child->GetOwn());

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

void Canvas::MouseCheckChild(RectTransform* child, bool down)
{
	GameObject* childObj = static_cast<GameObject*>(child->GetOwn());

	if (!childObj->IsActive()) return;

	// 子のUIObjectがアクティブなら

	// 逆順で再帰的にチェック
	std::vector<RectTransform*>& children = child->GetChildren();
	for (auto it = children.rbegin(); it != children.rend(); ++it)
	{
		MouseCheckChild(*it, down);
	}

	// 子をチェックした後に自分をチェック
	childObj->GetComponentsWithCategory(Category::UIBehavior, m_components);

	// コンポーネントを走査
	for (auto& behavior : m_components)
	{
		// アクティブなら
		if (behavior->IsActive())
		{
			// クリック時処理
			if (down) static_cast<UIBehaviorBase*>(behavior)->OnMouseDown();
			else static_cast<UIBehaviorBase*>(behavior)->OnMouseUp();
		}
	}
}
