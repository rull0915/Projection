/**
 * @file    GameInitializer.h
 * @author  Hoshino Ryunosuke
 * @date    2026/06/28
 *
 * @brief   ゲームの初期化をまとめたクラス
 */
#pragma once

/**
 * @brief ゲームの初期化をする
 */
class GameInitializer
{
private:
	// 初期化済みフラグ
	bool m_isInitialized;

public:
	GameInitializer()
		: m_isInitialized{ false }
	{
	}

	~GameInitializer() = default;

	// 初期化関数
	static void Initialize();

	// キーの追加
	static void AddKey();
};
