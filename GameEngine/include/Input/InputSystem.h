/**
 * @file    InputSystem.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/27
 *
 * @brief   入力システム管理クラス
 */

#pragma once

#include <unordered_map>
#include <string>
#include <variant>
#include <optional>
#include <vector>

#include "InputInfo.h"

namespace RHEngine
{
	namespace Input
	{
		/**
		 * @brief ボタン入力のバインド
		 */
		struct ButtonBinding
		{
			/**
			 * @brief ボタンと軸の両方に対応する
			 */
			std::variant<InputButton, InputAxis> input{};

			/**
			 * @brief 軸入力をボタンとして扱う際に押していることにするボーダー
			 * @details ボタンの際は扱わないことを明示するためにoptionalを使用
			 */
			std::optional<float> threshold{};
		};

		/**
		 * @brief 軸入力のバインド
		 */
		struct AxisBinding
		{
			/**
			 * @brief 軸入力 ボタン入力の場合は扱わないためoptinoalを使用
			 */
			std::optional<InputAxis> axis{};

			/**
			 * @brief マイナスとして扱うボタン入力
			 */
			std::optional<InputButton> negative{};

			/**
			 * @brief プラスとして扱うボタン入力
			 */
			std::optional<InputButton> positive{};

			/**
			 * @brief 入力にかけられる倍率
			 * @details マウスの移動量を軸入力としてを扱う際などに使用する
			 */
			float scale = 1.0f;
		};

		/**
		 * @brief カスタム入力を扱う
		 * @details プロジェクト全体で入力を統一するために静的クラスとして扱う
		 *			アクセス頻度が高く、毎回Instanceを取得するのが面倒のため通常のシングルトンパターンではなく全てを静的にしたクラスとした
		 */
		class Custom
		{
		private:
			
			/**
			 * @brief ユーザーが追加した軸入力の一覧
			 */
			inline static std::unordered_map<std::string, std::vector<AxisBinding>> m_axes;

			/**
			 * @brief ユーザーが追加したボタン入力の一覧
			 */
			inline static std::unordered_map<std::string, std::vector<ButtonBinding>> m_buttons;

		public:
			
			/**
			 * @brief ボタンが押されているかどうか
			 * @param key 調べるボタン入力名
			 * @return 押されているかどうか
			 */
			static bool GetButton(const std::string& key);

			/**
			 * @brief ボタンが押されたフレームかどうか
			 * @param key 調べるボタン入力名
			 * @return 押されたフレームかどうか
			 */
			static bool GetButtonDown(const std::string& key);

			/**
			 * @brief ボタンが離されたフレームかどうか
			 * @param key 調べるボタン入力名
			 * @return 離されたフレームかどうか
			 */
			static bool GetButtonUp(const std::string& key);

			/**
			 * @brief 軸入力の量を取得する
			 * @param key 取得する軸入力名
			 * @return 入力量
			 */
			static float GetAxis(const std::string& key);

			/**
			 * @brief ボタン入力を追加する
			 * @param key ボタン入力名
			 * @param button 対象のボタン
			 */
			static void AddButton(const std::string& key, const InputButton& button);

			/**
			 * @brief 軸をボタン入力として追加する
			 * @param key ボタン入力名
			 * @param axis 対象の軸
			 * @param threshold 押したことにするボーダー
			 */
			static void AddButton(const std::string& key, const InputAxis& axis, float threshold);

			/**
			 * @brief 軸入力を追加する
			 * @param key 軸入力名
			 * @param axis 対象の軸
			 * @param scale スケーリング値
			 */
			static void AddAxis(const std::string& key, const InputAxis& axis, float scale = 1.0f);

			/**
			 * @brief ボタンを軸入力として追加する
			 * @param key 軸入力名
			 * @param positive プラスとして扱うボタン
			 * @param negative マイナスとして扱うボタン
			 * @param scale スケーリング値
			 */
			static void AddAxis(const std::string& key, const std::optional<InputButton>& positive, const std::optional<InputButton>& negative, float scale = 1.0f);

		private:
			
			/**
			 * @brief ボタンのバインディングを追加する
			 * @param key ボタン入力名
			 * @param bind 追加するボタン
			 */
			static void AddButtonBinding(const std::string& key, const ButtonBinding& bind);

			/**
			 * @brief 軸のバインディングを追加する
			 * @param key 軸入力名
			 * @param bind 追加する軸
			 */
			static void AddAxisBinding(const std::string& key, const AxisBinding& bind);

			/**
			 * @brief 現在のボタン入力状況を取得する
			 * @param button 対象のボタン
			 * @return 押されているかどうか
			 */
			static bool GetButtonInput(InputButton button);

			/**
			 * @brief 前フレームのボタン入力状況を取得する
			 * @param button 対象のボタン
			 * @return 押されていたかどうか
			 */
			static bool GetOldButtonInput(InputButton button);

			/**
			 * @brief 現在の軸入力値を取得する
			 * @param axis 対象の軸
			 * @return 入力値
			 */
			static float GetAxisInput(InputAxis axis);

			/**
			 * @brief 前フレームの軸入力値を取得する
			 * @param axis 対象の軸
			 * @return 入力値
			 */
			static float GetOldAxisInput(InputAxis axis);

			/**
			 * @brief ボタンリストから押されているかどうかを調べる
			 * @param bindings 調べるボタンリスト
			 * @return 1つでも押されているかどうか
			 */
			static bool GetButtonState(const std::vector<ButtonBinding>& bindings);

			/**
			 * @brief ボタンリストから前フレームに押されていたかどうか調べる
			 * @param bindings 調べるボタンリスト
			 * @return 1つでも押されていたかどうか
			 */
			static bool GetOldButtonState(const std::vector<ButtonBinding>& bindings);
			
			/**
			 * @brief 軸リストから入力値を取得する
			 * @param bindings 調べる軸リスト
			 * @return 軸入力の最大値
			 */
			static float GetAxisState(const std::vector<AxisBinding>& bindings);
		};
	}
}	// namespace RHEngine
