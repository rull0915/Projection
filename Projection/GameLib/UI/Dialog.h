/**
 * @file    Dialog.h
 * @author  Hoshino Ryunosuke
 * @date    2026/08/25
 *
 * @brief   ダイアログの基底クラス
 */
#pragma once

#include "Components/UI/UIComponentBase.h"

/**
 * @brief ダイアログ
 */
class Dialog : public RHEngine::UIComponentBase
{
public:
	DECLARE_COMPONENT_TYPE(UIComponentBase);

private:

	// 開いているかどうか
	bool m_isOpen;

public:

	Dialog(RHEngine::IComponentOwner* own);
	~Dialog() = default;

	// 開く関数
	virtual void Show() {};

	// 隠す関数
	virtual void Hide() {};

	// 開閉チェック
	bool IsOpen() const
	{
		return m_isOpen;
	}

protected:

	// 開閉フラグ変更
	void SetOpen(bool open)
	{
		m_isOpen = open;
	}
};
