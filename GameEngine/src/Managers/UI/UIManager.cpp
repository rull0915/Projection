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
#include "UIManager.h"

#include "Renderer/Renderer.h"
#include "Components/UI/RectTransform/RectTransform.h"
#include "Components/UI/Behaviour/ButtonUI.h"

#include "Input/MouseInput.h"

#include "Debug/RectDebugRenderer.h"

//====================================================//
// 関数の実体宣言
//====================================================//

UIManager::UIManager(Scene* pScene)
	: m_needSort{ true }
	, m_pScene{ pScene }
{}

UIManager::~UIManager()
{}

/// <summary>
/// 更新関数
/// </summary>
void UIManager::Update(const GameTimer& gameTimer)
{
	// 予約済みを登録
	RegisterReserveCanvases();

	// ソートが必要な場合
	if (m_needSort) SortCanvas();

	// キャンバスを更新する
	for (auto& canvas : m_canvases)
	{
		if (canvas->IsActive())
		{
			canvas->Update(gameTimer);
		}
	}

	// rayの衝突を調べる
	CheckHitRay(Input::Mouse::GetScaledMousePoint());
}

void UIManager::LateUpdate(const GameTimer& gameTimer)
{
	// キャンバスを更新する
	for (auto& canvas : m_canvases)
	{
		if (canvas->IsActive())
		{
			canvas->LateUpdate(gameTimer);
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
		if (!it->get()->IsActive()) continue;

		// 衝突しているオブジェクトが見つかったら
		if (auto* hit = it->get()->HitTest(position))
		{
			hitRect = hit;
			break;
		}
	}

	// 衝突していれば
	if (hitRect)
	{
		// ボタンコンポーネントを持っていれば
		if (ButtonUI* button = hitRect->GetOwn()->GetComponent<ButtonUI>())
		{
			// Hover状態にする
			button->SetIsHovered(true);
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
		[](const std::unique_ptr<Canvas>& a,
			const std::unique_ptr<Canvas>& b)
		{
			return a->GetDrawOrder() < b->GetDrawOrder();
		});

	m_needSort = false;
}

void UIManager::RegisterReserveCanvases()
{
	for (auto& reserve : m_reserveCanvases)
	{
		m_canvases.push_back(std::move(reserve));
	}
	m_reserveCanvases.clear();
}

void UIManager::Draw(Renderer& renderer)
{
	// キャンバスを描画する
	for (auto& canvas : m_canvases)
	{
		if (canvas->IsActive())
		{
			canvas->Draw(renderer);
		}
	}
}

void UIManager::Finalize()
{
	// 全キャンバスの終了処理を呼び出す
	for (auto& canvas : m_canvases)
	{
		if (canvas->IsActive())
		{
			canvas->Finalize();
		}
	}
}

Canvas* UIManager::CreateCanvas()
{
	// 生成
	auto canvas = std::make_unique<Canvas>(this);
	Canvas* ptr = canvas.get();
	m_reserveCanvases.push_back(std::move(canvas));

	m_needSort = true;

	return ptr;
}

void UIManager::RemoveObjects()
{
	for (auto& canvas : m_canvases)
	{
		canvas->RemoveDeadComponent();
	}

	for (auto& canvas : m_canvases)
	{
		canvas->RemoveReserves();
	}
}

void UIManager::DebugDraw(Renderer& renderer, int color)
{
	// 全キャンバスをループ
	for (auto& canvas : m_canvases)
	{
		// 描画
		RectDebugRenderer::DebugDraw(
			canvas->GetAllObjects(), renderer, color
		);
	}
}
