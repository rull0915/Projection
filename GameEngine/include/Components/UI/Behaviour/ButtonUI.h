//====================================================//
// ファイル名   : ButtonUI.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/05/27
//
// 概要 : ボタンUIクラス
//
// 更新履歴 :
// 2026/05/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include <functional>
#include <unordered_map>
#include "UIBehaviorBase.h"

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//
	class ButtonUI : public UIBehaviorBase
	{
	public:

		// クリック時イベント
		using OnClickEvent = std::function<void()>;

		// クリック時イベント識別用ID
		using EventToken = int;

	private:

		//-----------------------------------------------------
		// メンバ変数
		//-----------------------------------------------------

		// 各状態の色

		DirectX::SimpleMath::Color m_normalColor;  // 通常時
		DirectX::SimpleMath::Color m_hoverdColor;  // ホバー時
		DirectX::SimpleMath::Color m_pressedColor; // 押されている時

		bool m_isPressed;   // 自分が押されているか

		// 押されたときの処理
		std::unordered_map<EventToken, OnClickEvent> m_onClicked;

	public:

		//-----------------------------------------------------
		// コンストラクタ / デストラクタ
		//-----------------------------------------------------
		ButtonUI(IComponentOwner* own);
		~ButtonUI();

		//-----------------------------------------------------
		// 公開関数
		//-----------------------------------------------------
		void Update(const GameTimer& gameTimer) override;

		// マウスクリック
		void OnMouseDown() override;
		void OnMouseUp() override;

		// イベント追加関数
		EventToken AddOnClicked(OnClickEvent event);

		// イベント削除関数
		void RemoveOnClicked(EventToken token);

		//-----------------------------------------------------
		// Type
		//-----------------------------------------------------

		COMPONENT_TYPE(ButtonUI, UIBehaviorBase);

		//-----------------------------------------------------
		// セッター
		//-----------------------------------------------------

		void SetNormalColor(DirectX::SimpleMath::Color color) { m_normalColor = color; }
		void SetHoveredColor(DirectX::SimpleMath::Color color) { m_hoverdColor = color; }
		void SetPressedColor(DirectX::SimpleMath::Color color) { m_pressedColor = color; }

	private:

		//-----------------------------------------------------
		// 内部実装
		//-----------------------------------------------------

	};
} // namespace REngine
