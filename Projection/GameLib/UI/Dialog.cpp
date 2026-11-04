/**
 * @file    Dialog.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/08/25
 */

#include "pch.h"
#include "Dialog.h"

// コンストラクタ
Dialog::Dialog(RHEngine::IComponentOwner* own)
	: UIComponentBase(own)
	, m_isOpen{ false }
{
}
