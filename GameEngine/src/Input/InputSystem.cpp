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

#include "Input/MouseInput.h"
#include "Input/KeyInput.h"

//====================================================//
// 関数の実体宣言
//====================================================//

namespace Input
{
	bool Custom::GetButton(const std::string& key)
	{
		// イテレータを取得
		auto it = m_buttons.find(key);

		// 存在しないキーなら
		if (it == m_buttons.end()) return false;

		// 入力チェック
		for (auto& input : it->second.inputs)
		{
			// 一つでも入力があれば
			if (GetBindInput(input))
			{
				// trueを返す
				return true;
			}
		}
		
		// 全入力がfalse
		return false;
	}

	int Custom::GetAxis(const std::string& key)
	{
		// イテレータを取得
		auto it = m_axes.find(key);

		// 存在しないキーなら
		if (it == m_axes.end()) return false;

		// 入力チェック
		int result = 0;

		// マイナス入力
		for (auto& input : it->second.negative)
		{
			// 1つでも入力があれば
			if (GetBindInput(input))
			{
				// 値を減らす
				result--;

				// ループ終了
				break;
			}
		}

		// プラス入力
		for (auto& input : it->second.positive)
		{
			// 1つでも入力があれば
			if (GetBindInput(input))
			{
				// 値を増やす
				result++;

				// ループ終了
				break;
			}
		}

		// 結果を返す
		return result;
	}

	void Custom::AddButton(const std::string& key, const Bind& bind)
	{
		// イテレータを取得
		auto it = m_buttons.find(key);

		// 存在しないキーなら
		if (it == m_buttons.end())
		{
			// 新しく追加
			m_buttons.insert({ key, Button{ { bind } } });
		}
		// 既にあるキーなら
		else
		{
			// 配列に追加
			it->second.inputs.push_back(bind);
		}
	}

	void Custom::AddAxis(const std::string& key, const Bind& bind, Axis::Side side)
	{
		// イテレータを取得
		auto it = m_axes.find(key);

		// 存在しないキーなら
		if (it == m_axes.end())
		{
			Axis axis;

			// 入力を追加
			(side == Axis::Side::Positive ? axis.positive : axis.negative).push_back(bind);

			// 配列に追加
			m_axes.insert({ key, axis });
		}
		// 既にあるキーなら
		else
		{
			// 配列に追加
			(side == Axis::Side::Positive ? it->second.positive : it->second.negative).push_back(bind);
		}
	}

	bool Custom::GetBindInput(Bind bind)
	{
		// デバイスタイプで分岐
		switch (bind.deviceType)
		{
			// キーボードの場合
		case Type::Key:
			return Key::Get(bind.state, static_cast<Key::Code>(bind.inputID));

			// マウスの場合
		case Type::Mouse:
			return Mouse::Get(bind.state, static_cast<Mouse::Button>(bind.inputID));

		default:
			return false;
		}
	}
}
