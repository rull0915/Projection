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

#include "GameLib/Input/MouseInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

UIManager::UIManager()
	: m_needSort{ true }
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
	CheckHitRay();
}

void UIManager::CheckHitRay()
{
	DirectX::SimpleMath::Vector2 mousePos = MouseInput::GetScaledMousePoint();

	RectTransform* hitRect = nullptr;

	// 描画の逆順で調べる
    for (auto it = m_canvases.rbegin();
         it != m_canvases.rend();
         ++it)
    {
		if (!it->get()->IsActive()) continue;

		// 衝突しているオブジェクトが見つかったら
        if (auto* hit = it->get()->HitTest(mousePos))
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

Canvas* UIManager::CreateCanvas()
{
    auto canvas = std::make_unique<Canvas>(this);
    Canvas* ptr = canvas.get();
    m_reserveCanvases.push_back(std::move(canvas));

	m_needSort = true;

    return ptr;
}
