//====================================================//
// ファイル名   : Canvas.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/08/27
//
// 概要 : キャンバスコンポーネント
//
// 更新履歴 :
// 2026/08/27 新規作成
//====================================================//

#pragma once

#define IS_COMPONENT(Canvas)

//====================================================//
// インクルードファイル
//====================================================//
#include "Components/UI/UIComponentBase.h"
#include "Components/UI/RectTransform/RectTransform.h"

namespace REngine
{
	//====================================================//
	// 前方宣言
	//====================================================//
	class Renderer;

	//====================================================//
	// クラス宣言
	//====================================================//
	class Canvas : public UIComponentBase
	{
	public:
		enum class MouseEvent
		{
			Down, Up
		};

	private:

		//-----------------------------------------------------
		// 定数
		//-----------------------------------------------------


		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 描画順
		int m_drawOrder;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		Canvas(IComponentOwner* own);
		~Canvas() = default;

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(Canvas, UIComponentBase)

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------

		void Awake() override;
		void Start() override;

		void Update(const GameTimer& gameTimer) override;
		void LateUpdate(const GameTimer& gameTimer) override;

		// 描画関数
		void Draw(Renderer& renderer);

		// 自身の子供の中で当たっているオブジェクトを返す関数
		RectTransform* CheckHit(DirectX::SimpleMath::Vector2 point);

		// マウスイベントを通知する関数
		void NotifyMouseEvent(MouseEvent event);

		//-----------------------------------------------------
		// ゲッター
		//-----------------------------------------------------

		int GetDrawOrder() const
		{
			return m_drawOrder;
		}

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetDrawOrder(int order)
		{
			m_drawOrder = order;
		}

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

		// 自身の子供を再帰的に描画する関数
		void DrawChild(RectTransform* rectTransform, Renderer& renderer);

		// 特定の点が当たるオブジェクトを走査する関数
		RectTransform* CheckHitPoint(RectTransform* rectTransform, DirectX::SimpleMath::Vector2 point);

		// マウスイベントを再帰的に通知する関数
		void MouseEventChild(RectTransform* rectTransform, MouseEvent event);
	};
}
