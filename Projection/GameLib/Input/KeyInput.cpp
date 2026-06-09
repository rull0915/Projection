//====================================================//
// ファイル名  : KeyInput.cpp
// 作成者      : Hoshino Ryunosuke
// 作成日       : 2026/02/16
//
// 概要        :  キー入力管理クラス
//====================================================//

//====================================================//
// インクルードファイル
//====================================================//
#include "pch.h"
#include "KeyInput.h"

//====================================================//
// static変数の実体宣言
//====================================================//
DirectX::Keyboard::State KeyInput::m_nowState;
DirectX::Keyboard::State KeyInput::m_oldState;

std::unordered_map<CustomType, std::pair<std::vector<KeyCode>, std::vector<KeyCode>>> KeyInput::m_custom;

//====================================================//
// 関数の実体宣言
//====================================================//

void KeyInput::Initialize()
{
	m_nowState = DirectX::Keyboard::Get().GetState();
	m_oldState = m_nowState;

	// カスタムマップにキーを追加する

	// 左右入力
	m_custom[CustomType::Horizontal] =
	{
		{ KeyCode::A, KeyCode::Left },
		{ KeyCode::D, KeyCode::Right }
	};
	// 上下入力
	m_custom[CustomType::Vertical] =
	{
		{ KeyCode::S, KeyCode::Down },
		{ KeyCode::W, KeyCode::Up }
	};
	// ジャンプ
	m_custom[CustomType::Jump] =
	{
		{},
		{ KeyCode::Space }
	};
}

void KeyInput::KeyUpdate()
{
	m_oldState = m_nowState;
	m_nowState = DirectX::Keyboard::Get().GetState();
}

float KeyInput::GetCustomInput(CustomType type)
{
	auto target = m_custom.find(type);

	// 指定されたタイプがcustomマップに設定されていなければスキップ
	if (target == m_custom.end()) return 0.0f;

	// 存在していれば

	// 入力結果出力用
	float result = 0.0f;

	// マイナスキーを調べる
	for (const auto& key : target->second.first)
	{
		// 押されていたら
		if (GetKey(key))
		{
			// リザルトを減らしループ終了
			result -= 1.0f;
			break;
		}
	}

	// プラスキーを調べる
	for (const auto& key : target->second.second)
	{
		// 押されていたら
		if (GetKey(key))
		{
			// リザルトを増やしループ終了
			result += 1.0f;
			break;
		}
	}

	return result;
}

float KeyInput::GetCustomInputUp(CustomType type)
{
	auto target = m_custom.find(type);

	// 指定されたタイプがcustomマップに設定されていなければスキップ
	if (target == m_custom.end()) return 0.0f;

	// 存在していれば

	// 入力結果出力用
	float result = 0.0f;

	// マイナスキーを調べる
	for (const auto& key : target->second.first)
	{
		// 離されていたら
		if (GetKeyUp(key))
		{
			// リザルトを減らしループ終了
			result -= 1.0f;
			break;
		}
	}

	// プラスキーを調べる
	for (const auto& key : target->second.second)
	{
		// 離されていたら
		if (GetKeyUp(key))
		{
			// リザルトを増やしループ終了
			result += 1.0f;
			break;
		}
	}

	return result;
}

float KeyInput::GetCustomInputDown(CustomType type)
{
	auto target = m_custom.find(type);

	// 指定されたタイプがcustomマップに設定されていなければスキップ
	if (target == m_custom.end()) return 0.0f;

	// 存在していれば

	// 入力結果出力用
	float result = 0.0f;

	// マイナスキーを調べる
	for (const auto& key : target->second.first)
	{
		// 押されていたら
		if (GetKeyDown(key))
		{
			// リザルトを減らしループ終了
			result -= 1.0f;
			break;
		}
	}

	// プラスキーを調べる
	for (const auto& key : target->second.second)
	{
		// 押されていたら
		if (GetKeyDown(key))
		{
			// リザルトを増やしループ終了
			result += 1.0f;
			break;
		}
	}

	return result;
}
