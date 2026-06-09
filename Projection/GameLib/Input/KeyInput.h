#pragma once
#include <Keyboard.h>

#include <unordered_map>

using KeyCode = DirectX::Keyboard::Keys;

// カスタム用列挙型
enum class CustomType
{
	Horizontal,
	Vertical,
	Jump,
};

class KeyInput
{
private:
	static DirectX::Keyboard::State m_nowState;
	static DirectX::Keyboard::State m_oldState;

	// ユーザーカスタム入力用のマップ
	static std::unordered_map<CustomType, std::pair<std::vector<KeyCode>, std::vector<KeyCode>>> m_custom;

public:
	// ----- 関数宣言 ----- //

	KeyInput();

	~KeyInput() = default;

	// 初期化関数
	static void Initialize();

	// キー情報の更新
	static void KeyUpdate();

	// --- キー情報取得関数 --- //

	// 押されたとき
	static inline bool GetKeyDown(DirectX::Keyboard::Keys key)
	{
		return !m_oldState.IsKeyDown(key) && m_nowState.IsKeyDown(key);
	}
	
	// 押されている間
	static inline bool GetKey(DirectX::Keyboard::Keys key)
	{
		return 	m_nowState.IsKeyDown(key);
	}

	// 離されたとき
	static inline bool GetKeyUp(DirectX::Keyboard::Keys key)
	{
		return m_oldState.IsKeyDown(key) && !m_nowState.IsKeyDown(key);
	}

	// カスタム入力の取得
	static float GetCustomInput(CustomType type);
	static float GetCustomInputUp(CustomType type);
	static float GetCustomInputDown(CustomType type);

	// カスタム入力の追加
	static inline void AddCustomInput(CustomType type, bool isMinus, KeyCode code)
	{
		// マイナスキーなら
		if (isMinus)
			// マイナスリストに追加
			m_custom[type].first.push_back(code);
		// プラスキーなら
		else	
			// プラスリストに追加
			m_custom[type].second.push_back(code);
	}

	// カスタム入力のリセット
	static inline void ResetCustomInput(CustomType type, bool isMinus)
	{
		if (isMinus)
			m_custom[type].first.clear();
		else
			m_custom[type].second.clear();
	}
};
