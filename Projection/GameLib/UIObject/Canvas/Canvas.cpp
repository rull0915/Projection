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
#include "Canvas.h"

#include "../UIComponents/Graphics/UIGraphic.h"
#include "../Manager/UIManager.h"

#include "DebugManager.h"

//====================================================//
// 関数の実体宣言
//====================================================//

Canvas::Canvas(UIManager* uiManager)
	: m_drawOrder{ 0 }
	, m_pUIManager{ uiManager }
	, m_pUIObjects{}
{
	// RectTransformの初期設定
	auto* rectTransform = GetComponent<RectTransform>();

	rectTransform->SetAnchor({ 0.5f, 0.5f });
	rectTransform->SetSize({ Screen::WIDTH, Screen::HEIGHT });

	rectTransform->SetAnchoredPosition({ 0, 0 });
	rectTransform->SetPivot({ 0.5f, 0.5f });
}

void Canvas::Update(const GameTimer& gameTimer)
{
	// 生成予約されたオブジェクトを登録
	RegisterReservations();

	// 自身の子を取得
	std::vector<RectTransform*>& children = GetComponent<RectTransform>()->GetChildren();

	// 全ての子に対して操作
	for (auto& child : children)
	{
		// 子を更新
		UpdateChild(child, gameTimer);
	}
}

void Canvas::Draw(Renderer& renderer)
{
	// 自身の子を取得
	std::vector<RectTransform*>& children = GetComponent<RectTransform>()->GetChildren();

	// 全ての子に対して操作
	for (auto& child : children)
	{
		// 子を描画
		DrawChild(child, renderer);
	}
}

void Canvas::Remove(UIObject* obj)
{
	// 指定されたオブジェクトを探す
	auto it = std::find_if(m_pUIObjects.begin(), m_pUIObjects.end(),
		[obj](const std::unique_ptr<UIObject>& ptr) { return ptr.get() == obj; });

	// リストにあれば削除
	if (it != m_pUIObjects.end())
	{
		m_pUIObjects.erase(it);
	}
}

/// <summary>
/// 指定した点が当たっているRectTransformを返す関数
/// </summary>
RectTransform* Canvas::HitTest(const DirectX::SimpleMath::Vector2& point)
{
	auto* rect = GetComponent<RectTransform>();

	auto& children = rect->GetChildren();

	for (auto it = children.rbegin();
		it != children.rend();
		++it)
	{
		if (auto* hit = HitTestChild(*it, point))
		{
			return hit;
		}
	}

	return nullptr;
}

void Canvas::SetDrawOrder(int order)
{
	if (m_drawOrder == order) return;

	m_drawOrder = order;

	// UIManagerに順番の更新を通知
	m_pUIManager->SetNeedSort(true);
}

void Canvas::DrawChild(RectTransform* child, Renderer& renderer)
{
	UIObject* childObj = child->GetOwn();

	if (!childObj->IsActive()) return;

	// 子のUIObjectがアクティブなら

	// Graphicを継承しているコンポーネントを取得
	for (auto graphic : childObj->GetComponents<BaseGraphic>())
	{
		// 描画
		if (graphic->IsActive()) graphic->Draw(renderer);
	}

	// RectTransformのデバッグ描画
	if(DebugManager::Instance().IsDrawRectTransform()) childObj->GetComponent<RectTransform>()->DebugRender(renderer);

	// 再帰的に描画
	for (auto grandChild : child->GetChildren())
	{
		DrawChild(grandChild, renderer);
	}
}
