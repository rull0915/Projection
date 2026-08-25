//====================================================//
// ファイル名   : InputSystem.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要 : 入力システム管理クラス
//
// 更新履歴 :
// 2026/06/27 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//
#include "InputInfo.h"

#include <unordered_map>
#include <string>
#include <variant>
#include <optional>

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//

	namespace Input
	{
		struct ButtonBinding
		{
			// ボタンと軸両対応
			std::variant<InputButton, InputAxis> input{};

			// 軸をボタンとして扱うときに押していることにするボーダー
			// ボタンの場合は扱わないことを明示するエラー値とするためにoptionalを使用します
			std::optional<float> threshold{};
		};
		struct AxisBinding
		{
			// ButtonとAxisに両対応
			// 扱わないもう片方をエラー値とするためにoptionalを使用します

			// Axis
			std::optional<InputAxis> axis{};

			// Button
			std::optional<InputButton> negative{};
			std::optional<InputButton> positive{};

			// スケール
			float scale = 1.0f;
		};

		class Custom
		{
		private:
			// ユーザー設定のキー入力を保持する変数
			inline static std::unordered_map<std::string, std::vector<AxisBinding>> m_axes;

			inline static std::unordered_map<std::string, std::vector<ButtonBinding>> m_buttons;

		public:
			// 入力を調べる関数
			static bool GetButton(const std::string& key);
			static bool GetButtonDown(const std::string& key);
			static bool GetButtonUp(const std::string& key);

			static float GetAxis(const std::string& key);

			// 入力を追加する関数
			static void AddButton(const std::string& key, const InputButton& button);
			static void AddButton(const std::string& key, const InputAxis& axis, float threshold);

			static void AddAxis(const std::string& key, const InputAxis& axis, float scale = 1.0f);
			static void AddAxis(const std::string& key, const std::optional<InputButton>& positive, const std::optional<InputButton>& negative, float scale = 1.0f);

		private:
			// 各Bindingの追加をする関数
			static void AddButtonBinding(const std::string& key, const ButtonBinding& bind);
			static void AddAxisBinding(const std::string& key, const AxisBinding& bind);

			// Buttonの入力状態を取得する関数
			static bool GetButtonInput(InputButton button);
			static bool GetOldButtonInput(InputButton button);

			// Axisの入力状態を取得する関数
			static float GetAxisInput(InputAxis axis);
			static float GetOldAxisInput(InputAxis axis);

			// Buttonバインドの状態を取得する関数
			static bool GetButtonState(const std::vector<ButtonBinding>& bindings);
			static bool GetOldButtonState(const std::vector<ButtonBinding>& bindings);
			
			// Axisバインドの状態を取得する関数
			static float GetAxisState(const std::vector<AxisBinding>& bindings);
		};
	}
}	// namespace REngine
