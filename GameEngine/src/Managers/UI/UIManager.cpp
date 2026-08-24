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
		: m_needSort{ true }
		, m_pScene{ pScene }
	{}

	UIManager::~UIManager()
	{}

	/// <summary>
	/// 更新関数
	/// </summary>
	void UIManager::Update(const GameTimer& gameTimer, bool playing)
	{
		// 予約済みを登録
		RegisterReserveCanvases();

		// 予約済みを削除
		RemoveDestroyedCanvas();

		// ソートが必要な場合
		if (m_needSort) SortCanvas();

		// キャンバスを更新する
		for (auto& canvas : m_canvases)
		{
			// キャンバスがアクティブなら
			if (canvas->IsActive())
			{
				// クリックされていれば
				if (Input::Mouse::GetDown(Input::Mouse::Button::Left))
				{
					canvas->OnMouseDown();
				}

				// 離されていれば
				if (Input::Mouse::GetUp(Input::Mouse::Button::Left))
				{
					canvas->OnMouseUp();
				}

				// 更新
				canvas->Update(gameTimer, playing);
			}
		}
	}

	void UIManager::LateUpdate(const GameTimer& gameTimer, bool playing)
	{
		// キャンバスを更新する
		for (auto& canvas : m_canvases)
		{
			if (canvas->IsActive())
			{
				canvas->LateUpdate(gameTimer, playing);
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
			static std::vector<ComponentBase*> base{};

			// ビヘイビアコンポーネントを取得
			hitRect->GetOwn()->GetComponentsWithCategory(Category::UIBehavior, base);

			for (auto& behavior : base)
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
			[](const std::unique_ptr<Canvas>& a,
				const std::unique_ptr<Canvas>& b)
			{
				return a->GetDrawOrder() < b->GetDrawOrder();
			});

		m_needSort = false;
	}

	void UIManager::RegisterReserveCanvases()
	{
		for (auto& reserve : m_addReserves)
		{
			m_canvases.push_back(std::move(reserve));
		}
		m_addReserves.clear();
	}

	void UIManager::RemoveDestroyedCanvas()
	{
		std::erase_if(m_canvases, [](const std::unique_ptr<Canvas>& canvas) { return canvas->IsDestroy(); });
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
			canvas->Finalize();
		}
	}

	Canvas* UIManager::CreateCanvas()
	{
		// 生成
		auto canvas = std::make_unique<Canvas>(this);
		Canvas* ptr = canvas.get();
		m_addReserves.push_back(std::move(canvas));

		m_needSort = true;

		return ptr;
	}

	void UIManager::RemoveObjects()
	{
		// 全キャンバスを調べる
		for (auto& canvas : m_canvases)
		{
			canvas->RemoveDeadComponent();
		}

		for (auto& canvas : m_canvases)
		{
			canvas->RemoveReserves();
		}
	}

	void UIManager::DebugDraw(Renderer& renderer, DirectX::SimpleMath::Color color)
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
}	// namespace REngine
