//====================================================//
// ファイル名  : Canvas.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/08/27
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Components/UI/Canvas.h"
#include "GameObject/GameObject.h"

#include "Components/UI/Graphics/UIGraphicBase.h"
#include "Components/UI/Behaviour/UIBehaviorBase.h"
#include "Components/UI/Graphics/ImageUI.h"

#include "System/WindowManager.h"

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	// コンストラクタ
	Canvas::Canvas(REngine::IComponentOwner* own)
		: UIComponentBase(own)
		, m_drawOrder{ 0 }
	{
		ADD_PROPERTY(Canvas, m_drawOrder);

		// ルートRectTransformの初期設定
		auto* rectTransform = GetComponent<RectTransform>();

		rectTransform->SetAnchor({ 0.5f, 0.5f });
		rectTransform->SetSize({ WindowManager::Instance().GetWidthF(), WindowManager::Instance().GetHeightF() });
		rectTransform->SetAnchoredPosition({ 0, 0 });
		rectTransform->SetPivot({ 0.5f, 0.5f });
	}

	// 生成直後に一度呼ばれます
	void Canvas::Awake()
	{
	}

	// 最初のUpdate関数の直線に一度呼ばれます
	void Canvas::Start()
	{}

	// 毎フレーム呼ばれます
	void Canvas::Update(const REngine::GameTimer& gameTimer)
	{}

	// 毎フレームUpdate及び物理挙動の後に呼ばれます
	void Canvas::LateUpdate(const REngine::GameTimer& gameTimer)
	{}

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

	void Canvas::DrawChild(RectTransform* rectTransform, Renderer& renderer)
	{
		// GameObjectを取得
		GameObject* obj = static_cast<GameObject*>(rectTransform->GetOwn());

		// 非アクティブなら描画しない
		if (!obj->IsActive()) return;

		// Graphicを継承しているコンポーネントを取得
		for (auto graphic : obj->GetComponents<UIGraphicBase>())
		{
			// 描画
			if (graphic->IsActive()) static_cast<UIGraphicBase*>(graphic)->Draw(renderer);
		}

		// 子供を探索
		for (auto grandChild : rectTransform->GetChildren())
		{
			// 再帰的に描画
			DrawChild(grandChild, renderer);
		}
	}

	RectTransform* Canvas::CheckHitPoint(RectTransform* rectTransform, DirectX::SimpleMath::Vector2 point)
	{
		// オブジェクトを取得
		GameObject* obj = static_cast<GameObject*>(rectTransform->GetOwn());

		// 非アクティブなら調べない
		if (!obj->IsActive()) return nullptr;

		// 逆順で再帰的にチェック
		std::vector<RectTransform*>& children = rectTransform->GetChildren();
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
		if (auto* image = obj->GetComponent<ImageUI>())
		{
			// raycastと当たる設定なら
			if (!image->IsActive() || !image->IsRaycastTarget()) return nullptr;

			// 点が自分の上にあれば
			if (rectTransform->Contains(point))
			{
				// 自分を返す
				return rectTransform;
			}
		}

		return nullptr;
	}

	void Canvas::MouseEventChild(RectTransform* rectTransform, MouseEvent event)
	{
		// オブジェクトを取得
		GameObject* childObj = static_cast<GameObject*>(rectTransform->GetOwn());

		if (!childObj->IsActive()) return;

		// 子のUIObjectがアクティブなら

		// 逆順で再帰的にチェック
		std::vector<RectTransform*>& children = rectTransform->GetChildren();
		for (auto it = children.rbegin(); it != children.rend(); ++it)
		{
			MouseEventChild(*it, event);
		}

		// 子をチェックした後に自分をチェック
		for (auto& behavior : childObj->GetComponents<UIBehaviorBase>())
		{
			// アクティブなら
			if (behavior->IsActive())
			{
				// イベント別処理
				switch (event)
				{
				case REngine::Canvas::MouseEvent::Down:
					static_cast<UIBehaviorBase*>(behavior)->OnMouseDown();
					break;
				case REngine::Canvas::MouseEvent::Up:
					static_cast<UIBehaviorBase*>(behavior)->OnMouseUp();
					break;
				default:
					break;
				}
			}
		}
	}
}
