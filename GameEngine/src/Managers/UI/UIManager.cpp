/**
 * @file    UIManager.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/05/24

 */
#include "pch.h"
#include "Managers/UI/UIManager.h"

#include "Renderer/Renderer.h"
#include "Components/UI/RectTransform/RectTransform.h"
#include "Components/UI/Behaviour/ButtonUI.h"

#include "Input/MouseInput.h"

namespace RHEngine
{
	UIManager::UIManager()
	{}

	UIManager::~UIManager()
	{}

	void UIManager::CheckEvent()
	{
		// キャンバスを更新する
		for (auto& pCanvas : GetObjects())
		{
			// キャンバスがアクティブなら
			if (pCanvas->IsActive())
			{
				// クリックされていれば
				if (Input::Mouse::GetDown(Input::Mouse::Button::Left))
				{
					pCanvas->NotifyMouseEvent(Canvas::MouseEvent::Down);
				}

				// 離されていれば
				if (Input::Mouse::GetUp(Input::Mouse::Button::Left))
				{
					pCanvas->NotifyMouseEvent(Canvas::MouseEvent::Up);
				}
			}
		}
	}

	void UIManager::CheckHitRay(DirectX::SimpleMath::Vector2 position)
	{
		// 当たったRect
		RectTransform* pHitRect = nullptr;

		// 描画の逆順で調べる
		for (auto it = GetObjects().rbegin();
			it != GetObjects().rend();
			++it)
		{
			if (!(*it)->IsActive()) continue;

			// 衝突しているオブジェクトが見つかったら
			if (auto* pHit = (*it)->CheckHit(position))
			{
				pHitRect = pHit;
				break;
			}
		}

		// 衝突していれば
		if (pHitRect)
		{
			// ビヘイビアコンポーネントを取得
			for (auto& pBehavior : pHitRect->GetOwn()->GetComponents<UIBehaviorBase>())
			{
				// Hover状態にする
				static_cast<UIBehaviorBase*>(pBehavior)->SetHovered(true);
			}
		}
	}

	void UIManager::SortCanvas()
	{
		// 同値のキャンバスは順序を保持
		std::stable_sort(
			GetObjects().begin(), // 最初から
			GetObjects().end(),   // 最後まで 
			// 描画順で入れ替え
			[](const Canvas* a,
				const Canvas* b)
			{
				return a->GetDrawOrder() < b->GetDrawOrder();
			});
	}

	void UIManager::Draw(Renderer& renderer)
	{
		// ソート
		SortCanvas();

		// キャンバスを描画する
		for (auto& pCanvas : GetObjects())
		{
			if (pCanvas->IsActive())
			{
				pCanvas->Draw(renderer);
			}
		}
	}

	void UIManager::DebugDraw(Renderer& renderer, DirectX::SimpleMath::Color color)
	{
		// 全キャンバスをループ
		//for (auto& canvas : GetObjects())
		//{
		//	// 描画
		//	RectDebugRenderer::DebugDraw(
		//		canvas->GetAllObjects(), renderer, color
		//	);
		//}
	}
}	// namespace RHEngine
