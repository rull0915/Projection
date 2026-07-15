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
#include <vector>
#include <unordered_map>
#include <string>

namespace REngine
{
	//====================================================//
	// クラス宣言
	//====================================================//

	namespace Input
	{
		enum class Type
		{
			Key,
			Mouse,
		};

		struct Bind
		{
			// 入力デバイスの種類
			Type deviceType;

			// 入力の状態
			State state;

			// 入力のID
			ID inputID;
		};

		struct Axis
		{
			std::vector<Bind> negative;
			std::vector<Bind> positive;

			enum class Side : bool
			{
				Positive = true,
				Negative = false
			};
		};

		struct Button
		{
			std::vector<Bind> inputs;
		};

		class Custom
		{
		private:
			// ユーザー設定のキー入力を保持する変数
			inline static std::unordered_map<std::string, Axis> m_axes;

			inline static std::unordered_map<std::string, Button> m_buttons;

		public:
			// 入力を調べる関数
			static bool GetButton(const std::string& key);

			static int GetAxis(const std::string& key);

			// 入力を追加する関数
			static void AddButton(const std::string& key, const Bind& bind);

			static void AddAxis(const std::string& key, const Bind& bind, Axis::Side side);

		private:
			// Bindの入力状態を取得する関数
			static bool GetBindInput(Bind bind);
		};
	}
}	// namespace REngine
