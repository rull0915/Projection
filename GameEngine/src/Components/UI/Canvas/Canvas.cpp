/**
 * @file    Canvas.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/08/27
 */

#include "pch.h"
#include "Components/UI/Canvas/Canvas.h"

#include "GameObject/GameObject.h"
#include "Components/UI/Graphics/UIGraphicBase.h"
#include "Components/UI/Behaviour/UIBehaviorBase.h"
#include "Components/UI/Graphics/ImageUI.h"
#include "System/WindowManager.h"

namespace RHEngine
{
	// コンストラクタ
	Canvas::Canvas(RHEngine::IComponentOwner* pOwner)
		: UIComponentBase(pOwner)
		, m_drawOrder{ 0 }
	{
		ADD_PROPERTY(Canvas, m_drawOrder);

		// ルートRectTransformの初期設定
		auto* pRectTransform = GetComponent<RectTransform>();

		pRectTransform->SetAnchor({ 0.5f, 0.5f });
		pRectTransform->SetSize(WindowManager::Instance().GetSize());
		pRectTransform->SetAnchoredPosition({ 0, 0 });
		pRectTransform->SetPivot({ 0.5f, 0.5f });
	}

	void Canvas::Draw(Renderer& renderer)
	{
		// 自分をルートとして再帰的に描画する
		DrawChild(GetComponent<RectTransform>(), renderer);
	}

	RectTransform* Canvas::CheckHit(DirectX::SimpleMath::Vector2 point)
	{
		// 自分をルートとして再帰的に調べる
		return CheckHitPoint(GetComponent<RectTransform>(), point);
	}

	void Canvas::NotifyMouseEvent(MouseEvent event)
	{
		// 自分をルートとして再帰的に調べる
		MouseEventChild(GetComponent<RectTransform>(), event);
	}

	void Canvas::OnWindowSizeChanged(int width, int height)
	{
		GetComponent<RectTransform>()->SetSize(WindowManager::Instance().GetSize());
	}

	void Canvas::DrawChild(RectTransform* pRectTransform, Renderer& renderer)
	{
		// GameObjectを取得
		GameObject* pObject = static_cast<GameObject*>(pRectTransform->GetOwn());

		// 非アクティブなら描画しない
		if (!pObject->IsActive()) return;

		// Graphicを継承しているコンポーネントを取得
		for (auto* pGraphic : pObject->GetComponents<UIGraphicBase>())
		{
			// 描画
			if (pGraphic->IsActive()) static_cast<UIGraphicBase*>(pGraphic)->Draw(renderer);
		}

		// 子供を探索
		for (auto* pGrandChild : pRectTransform->GetChildren())
		{
			// 再帰的に描画
			DrawChild(pGrandChild, renderer);
		}
	}

	RectTransform* Canvas::CheckHitPoint(RectTransform* pRectTransform, DirectX::SimpleMath::Vector2 point)
	{
		// オブジェクトを取得
		GameObject* pObject = static_cast<GameObject*>(pRectTransform->GetOwn());

		// 非アクティブなら調べない
		if (!pObject->IsActive()) return nullptr;

		// 逆順で再帰的にチェック
		std::vector<RectTransform*>& children = pRectTransform->GetChildren();
		for (auto it = children.rbegin(); it != children.rend(); ++it)
		{
			// 当たっているオブジェクトが見つかれば
			if (auto* hit = CheckHitPoint(*it, point))
			{
				// それを返す
				return hit;
			}
		}

		// 子をチェックした後に自分をチェック

		// 自分にImageがあるなら
		if (auto* pImage = pObject->GetComponent<ImageUI>())
		{
			// raycastと当たる設定なら
			if (!pImage->IsActive() || !pImage->IsRaycastTarget()) return nullptr;

			// 点が自分の上にあれば
			if (pRectTransform->Contains(point))
			{
				// 自分を返す
				return pRectTransform;
			}
		}

		return nullptr;
	}

	void Canvas::MouseEventChild(RectTransform* pRectTransform, MouseEvent event)
	{
		// オブジェクトを取得
		GameObject* pChildObj = static_cast<GameObject*>(pRectTransform->GetOwn());

		if (!pChildObj->IsActive()) return;

		// 子のUIObjectがアクティブなら

		// 逆順で再帰的にチェック
		std::vector<RectTransform*>& children = pRectTransform->GetChildren();
		for (auto it = children.rbegin(); it != children.rend(); ++it)
		{
			MouseEventChild(*it, event);
		}

		// 子をチェックした後に自分をチェック
		for (auto& behavior : pChildObj->GetComponents<UIBehaviorBase>())
		{
			// アクティブなら
			if (behavior->IsActive())
			{
				// イベント別処理
				switch (event)
				{
				case RHEngine::Canvas::MouseEvent::Down:
					static_cast<UIBehaviorBase*>(behavior)->OnMouseDown();
					break;
				case RHEngine::Canvas::MouseEvent::Up:
					static_cast<UIBehaviorBase*>(behavior)->OnMouseUp();
					break;
				default:
					break;
				}
			}
		}
	}
}	// namespace RHEngine
