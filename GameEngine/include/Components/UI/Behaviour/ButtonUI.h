/**
 * @file    ButtonUI.h
 * @author  Hoshino Ryunosuke
 * @date    2026/05/27
 *
 * @brief   ボタンUIクラス
 */

#pragma once

#include <functional>
#include <unordered_map>

#include "UIBehaviorBase.h"

namespace RHEngine
{
	class GameTimer;
	class IComponentOwner;

	/**
	 * @brief ボタンUI
	 */
	class ButtonUI : public UIBehaviorBase
	{
	public:
		DECLARE_COMPONENT_TYPE(UIBehaviorBase);

		/** @brief クリック時イベント */
		using OnClickEvent = std::function<void()>;

		/** @brief クリック時イベントの識別用トークン */
		using EventToken = int;

	private:

		/// @name 各状態時の色

		/** @brief 通常時 */
		DirectX::SimpleMath::Color m_normalColor; 

		/** @brief ホバー時 (カーソルが自身に重なっているとき) */
		DirectX::SimpleMath::Color m_hoverdColor;

		/** @brief 押されているとき */
		DirectX::SimpleMath::Color m_pressedColor;

		/** @brief 現在押されているかどうか */
		bool m_isPressed;

		/** @brief 押されたときのコールバックリスト */
		std::unordered_map<EventToken, OnClickEvent> m_onClicked;

	public:

		/// @name ライフサイクル
		/// @{

		/**
		 * @param pOwner 所有者のポインタ
		 */
		ButtonUI(IComponentOwner* pOwner);

		~ButtonUI();

		/// @}

		/**
		 * @brief ボタンの状態に応じてImageUIの色変更を行う
		 * @param gameTimer 
		 */
		void Update(const GameTimer& gameTimer) override;

		/**
		 * @brief マウスクリック時自身にカーソルが重なっているか調べ状態変更を行う
		 */
		void OnMouseDown() override;

		/**
		 * @brief マウスを離した際に自身の状態を変更し、必要に応じてコールバックの呼び出しを行う
		 */
		void OnMouseUp() override;

		/**
		 * @brief クリック時コールバックを追加する
		 * @param event 呼び出されるコールバック関数
		 * @return 識別用トークン (削除時に使用)
		 */
		EventToken AddOnClicked(OnClickEvent event);

		/**
		 * @brief イベントを削除する
		 * @param token 追加時に発行された識別トークン
		 */
		void RemoveOnClicked(EventToken token);

		/**
		 * @brief クリック時コールバックを外部から呼び出す
		 */
		void Submit();

		/// @name セッター
		/// @{

		/** @brief 通常時の色 */
		void SetNormalColor(DirectX::SimpleMath::Color color) { m_normalColor = color; }

		/** @brief ホバー時の色 */
		void SetHoveredColor(DirectX::SimpleMath::Color color) { m_hoverdColor = color; }

		/** @brief 押下時の色 */
		void SetPressedColor(DirectX::SimpleMath::Color color) { m_pressedColor = color; }

		/// @}
	};
} // namespace RHEngine
