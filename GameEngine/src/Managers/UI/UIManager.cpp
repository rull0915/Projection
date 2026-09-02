//====================================================//
// ファイル名  : UIManager.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/05/24
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Managers/UI/UIManager.h"

#include "Renderer/Renderer.h"
#include "Components/UI/RectTransform/RectTransform.h"
#include "Components/UI/Behaviour/ButtonUI.h"

#include "Input/MouseInput.h"

#include "Debug/RectDebugRenderer.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	UIManager::UIManager(Scene* pScene)
	{}

	UIManager::~UIManager()
	{}

	/// <summary>
	/// 更新関数
	/// </summary>
	void UIManager::CheckEvent()
	{
		// キャンバスを更新する
		for (auto& canvas : m_canvases)
		{
			// キャンバスがアクティブなら
			if (canvas->IsActive())
			{
				// クリックされていれば
				if (Input::Mouse::GetDown(Input::Mouse::Button::Left))
				{
					canvas->NotifyMouseEvent(Canvas::MouseEvent::Down);
				}

				// 離されていれば
				if (Input::Mouse::GetUp(Input::Mouse::Button::Left))
				{
					canvas->NotifyMouseEvent(Canvas::MouseEvent::Up);
				}
			}
		}
	}

	void UIManager::CheckHitRay(DirectX::SimpleMath::Vector2 position)
	{
		// 当たったRect
		RectTransform* hitRect = nullptr;

		// 描画の逆順で調べる
		for (auto it = m_canvases.rbegin();
			it != m_canvases.rend();
			++it)
		{
			if (!(*it)->IsActive()) continue;

			// 衝突しているオブジェクトが見つかったら
			if (auto* hit = (*it)->CheckHit(position))
			{
				hitRect = hit;
				break;
			}
		}

		// 衝突していれば
		if (hitRect)
		{
			// ビヘイビアコンポーネントを取得
			for (auto& behavior : hitRect->GetOwn()->GetComponents<UIBehaviorBase>())
			{
				// Hover状態にする
				static_cast<UIBehaviorBase*>(behavior)->SetHovered(true);
			}
		}
	}

	void UIManager::SortCanvas()
	{
		// 同値のキャンバスは順序を保持
		std::stable_sort(
			m_canvases.begin(), // 最初から
			m_canvases.end(),   // 最後まで 
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
		for (auto& canvas : m_canvases)
		{
			if (canvas->IsActive())
			{
				canvas->Draw(renderer);
			}
		}
	}

	void UIManager::DebugDraw(Renderer& renderer, DirectX::SimpleMath::Color color)
	{
		// 全キャンバスをループ
		//for (auto& canvas : m_canvases)
		//{
		//	// 描画
		//	RectDebugRenderer::DebugDraw(
		//		canvas->GetAllObjects(), renderer, color
		//	);
		//}
	}
}	// namespace REngine
