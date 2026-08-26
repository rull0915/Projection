//====================================================//
// ファイル名  : InputSystem.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/06/27
//
// 概要       : 
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "Input/InputSystem.h"

#include <type_traits>

namespace REngine
{
	//====================================================//
	// 関数の実体宣言
	//====================================================//

	namespace Input
	{
		// 押されているとき
		bool Custom::GetButton(const std::string& key)
		{
			// イテレータを取得
			auto it = m_buttons.find(key);

			// 存在しないキーなら
			if (it == m_buttons.end()) return false;

			return GetButtonState(it->second);
		}

		// 押された瞬間
		bool Custom::GetButtonDown(const std::string& key)
		{
			// イテレータを取得
			auto it = m_buttons.find(key);

			// 存在しないキーなら
			if (it == m_buttons.end()) return false;

			return !GetOldButtonState(it->second) && GetButtonState(it->second);
		}

		// 離された瞬間
		bool Custom::GetButtonUp(const std::string& key)
		{
			// イテレータを取得
			auto it = m_buttons.find(key);

			// 存在しないキーなら
			if (it == m_buttons.end()) return false;

			return GetOldButtonState(it->second) && !GetButtonState(it->second);
		}

		float Custom::GetAxis(const std::string& key)
		{
			// イテレータを取得
			auto it = m_axes.find(key);

			// 存在しないキーなら
			if (it == m_axes.end()) return 0.0f;

			// 結果を返す
			return GetAxisState(it->second);
		}

		void Custom::AddButton(const std::string& key, const InputButton& button)
		{
			// バインド構造体を作成
			ButtonBinding bind{};
			bind.input = button;

			AddButtonBinding(key, bind);
		}

		void Custom::AddButton(const std::string& key, const InputAxis& axis, float threshold)
		{
			// バインド構造体を作成
			ButtonBinding bind{};
			bind.input = axis;
			bind.threshold = threshold;

			AddButtonBinding(key, bind);
		}

		void Custom::AddAxis(const std::string& key, const InputAxis& axis)
		{
			// バインド構造体を作成
			AxisBinding bind{};
			bind.axis = axis;

			AddAxisBinding(key, bind);
		}

		void Custom::AddAxis(const std::string& key, const std::optional<InputButton>& positive, const std::optional<InputButton>& negative)
		{
			// 両方設定されていないなら追加しない
			if (!positive.has_value() && !negative.has_value()) return;

			// バインド構造体を作成
			AxisBinding bind{};
			bind.positive = positive;
			bind.negative = negative;

			AddAxisBinding(key, bind);
		}

		void Custom::AddButtonBinding(const std::string& key, const ButtonBinding& bind)
		{
			// イテレータを取得
			auto it = m_buttons.find(key);

			// 存在しないキーなら
			if (it == m_buttons.end())
			{
				// 新しく追加
				m_buttons.insert({ key, { bind } });
			}
			// 既にあるキーなら
			else
			{
				// 配列に追加
				it->second.push_back(bind);
			}
		}

		void Custom::AddAxisBinding(const std::string& key, const AxisBinding& bind)
		{
			// イテレータを取得
			auto it = m_axes.find(key);

			// 存在しないキーなら
			if (it == m_axes.end())
			{
				// 新しく追加
				m_axes.insert({ key, { bind } });
			}
			// 既にあるキーなら
			else
			{
				// 配列に追加
				it->second.push_back(bind);
			}
		}

		bool Custom::GetButtonInput(InputButton button)
		{
			return std::visit([](auto input) {
				// inputの型
				using T = std::decay_t<decltype(input)>;

				// キーボードの場合
				if constexpr (std::is_same_v<T, Key::Code>)
					return Key::Get(input);
				// マウスの場合
				else if constexpr (std::is_same_v<T, Mouse::Button>)
					return Mouse::Get(input);
				// パッドの場合
				else if constexpr (std::is_same_v<T, Pad::Button>)
					return Pad::Get(input);
				else return false;
				}, button);
		}

		bool Custom::GetOldButtonInput(InputButton button)
		{
			return std::visit([](auto input) {
				// inputの型
				using T = std::decay_t<decltype(input)>;

				// キーボードの場合
				if constexpr (std::is_same_v<T, Key::Code>)
					return Key::GetOld(input);
				// マウスの場合
				else if constexpr (std::is_same_v<T, Mouse::Button>)
					return Mouse::GetOld(input);
				// パッドの場合
				else if constexpr (std::is_same_v<T, Pad::Button>)
					return Pad::GetOld(input);
				else return false;
				}, button
			);
		}

		float Custom::GetAxisInput(InputAxis axis)
		{
			return std::visit([](auto input) {
				// inputの型
				using T = std::decay_t<decltype(input)>;

				// マウスの場合
				if constexpr (std::is_same_v<T, Mouse::Axis>)
					return Mouse::GetMoveValue(input);
				// パッドの場合
				else if constexpr (std::is_same_v<T, Pad::Axis>)
					return Pad::GetCurrentAxis(input);
				}, axis);
		}

		float Custom::GetOldAxisInput(InputAxis axis)
		{
			return std::visit([](auto input) {
				// inputの型
				using T = std::decay_t<decltype(input)>;

				// マウスの場合
				if constexpr (std::is_same_v<T, Mouse::Axis>)
					return Mouse::GetOldMoveValue(input);
				// パッドの場合
				else if constexpr (std::is_same_v<T, Pad::Axis>)
					return Pad::GetOldAxis(input);
				}, axis);
		}

		bool Custom::GetButtonState(const std::vector<ButtonBinding>& bindings)
		{
			// 全入力を調べる
			for (auto& button : bindings)
			{
				// 押下フラグ
				if (std::visit([&button](auto input)
					{
						using T = std::decay_t<decltype(input)>;

						// ボタンの場合
						if constexpr (std::is_same_v<T, InputButton>)
							return GetButtonInput(input);
						// 軸の場合
						else if constexpr (std::is_same_v<T, InputAxis>)
							return GetAxisInput(input) >= button.threshold;
					}, button.input
				))
				// 1つでも押されていたらその時点でtrue
				return true;
			}

			// 全入力がfalse
			return false;
		}

		bool Custom::GetOldButtonState(const std::vector<ButtonBinding>& bindings)
		{
			// 全入力を調べる
			for (auto& button : bindings)
			{
				if (std::visit([&button](auto input)
					{
						using T = std::decay_t<decltype(input)>;

						// ボタンの場合
						if constexpr (std::is_same_v<T, InputButton>)
							return GetOldButtonInput(input);
						// 軸の場合
						else if constexpr (std::is_same_v<T, InputAxis>)
							return GetOldAxisInput(input) >= button.threshold;
					}, button.input
				))
				return true;
			}

			// 全入力がfalse
			return false;
		}

		float Custom::GetAxisState(const std::vector<AxisBinding>& bindings)
		{
			// 最終的な値
			float result = 0;

			// 全入力を調べる
			for (auto& bind : bindings)
			{
				// axisの時
				if (bind.axis.has_value())
				{
					// 値を取得
					float i = GetAxisInput(bind.axis.value());

					// 絶対値が上回っていれば更新
					if (std::abs(i) > std::abs(result)) result = i;
				}

				// 2つのButtonの場合
				else
				{
					// positiveの押下状況を取得
					bool p = bind.positive.has_value() ? GetButtonInput(bind.positive.value()) : false;

					// negativeの押下状況を取得
					bool n = bind.negative.has_value() ? GetButtonInput(bind.negative.value()) : false;

					// 最終的な値を算出
					float i = (p ? 1.0f : 0) + (n ? -1.0f : 0);

					// 絶対値が上回っていれば更新
					if (std::abs(i) > std::abs(result)) result = i;
				}
			}

			return result;
		}
	}
}	// namespace REngine
