/**
 * @file    ObjectForTree2D.cpp
 * @author  Hoshino Ryunosuke
 * @date    2026/04/16
 */

#include "pch.h"
#include "ObjectForTree2D.h"

#include "Cell2D.h"

namespace RHEngine
{
	bool ObjectForTree2D::Remove()
	{
		// 空間に属していなければ終了
		if (!m_pCell)
			return false;

		// 自分を登録している空間に自身を通知
		if (!m_pCell->OnRemove(this))
			return false;

		// 前後のポインタを繋ぎ直す
		if (m_pPre) m_pPre->m_pNext = m_pNext;
		if (m_pNext) m_pNext->m_pPre = m_pPre;

		// 自身のリンクを掃除
		m_pPre = nullptr;
		m_pNext = nullptr;
		m_pCell = nullptr;

		return true;
	}
}	// namespace RHEngine
