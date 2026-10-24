//====================================================//
// ファイル名   : Range.h
// 作成者       : Hoshino Ryunosuke
// 作成日       : 2026/10/23
//
// 概要 : 範囲クラス
//
// 更新履歴 :
// 2026/10/23 新規作成
//====================================================//

#pragma once

//====================================================//
// インクルードファイル
//====================================================//

#include <algorithm>

namespace RHEngine
{
	class Range
	{
	private:
		const float m_min;
		const float m_max;

		float m_value;	// 値

	public:
		Range(float a, float b)
			: m_min{ std::min(a, b) }
			, m_max{ std::max(a, b) }
			, m_value{ (a + b) / 2 }
		{}

		Range& operator=(const float& o)
		{
			m_value = std::clamp(o, m_min, m_max);
			return *this;
		}

		Range& operator=(const Range& rhs)
		{
			if (this != &rhs)
			{
				m_value = std::clamp(rhs.m_value, m_min, m_max);
			}
			return *this;
		}

		// floatへの暗黙変換を許可
		operator float() const { return m_value; }

		float GetMin() const { return m_min; }
		float GetMax() const { return m_max; }

		float GetValue() const { return m_value; }
	};
}
