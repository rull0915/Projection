/**
 * @file    Easing.h
 * @author  Hoshino Ryunosuke
 * @date    2026/02/24
 *
 * @brief   イージング関数をまとめたヘッダ
 * @details 0.0 ~ 1.0に正規化された値を引数に渡すことで変換後の値を取得できる
 */

#pragma once

#include <cmath>

namespace RHEngine
{
	/**
	 * @brief イージング関数をまとめた構造体
	 */
	struct Easing
	{
	private:
		static constexpr float PI_F = 3.141592653f;
		static constexpr float C1 = 1.70158f;
		static constexpr float C2 = C1 * 1.525f;
		static constexpr float C3 = C1 + 1.0f;
		static constexpr float C4 = (2.0f * PI_F) / 3.0f;
		static constexpr float C5 = (2.0f * PI_F) / 4.5f;

	public:

		/// @name Linear
		/// @{
		
		static float Linear(float t) { return t; }

		/// @}

		/// @name Sine
		/// @{
		
		static float EaseInSine(float t)
		{
			return 1.0f - cosf((t * PI_F) / 2.0f);
		}

		static float EaseOutSine(float t)
		{
			return sinf((t * PI_F) / 2.0f);
		}

		static float EaseInOutSine(float t)
		{
			return -(cosf(PI_F * t) - 1.0f) / 2.0f;
		}

		/// @}

		/// @name Quad
		/// @{
		
		static float EaseInQuad(float t)
		{
			return t * t;
		}

		static float EaseOutQuad(float t)
		{
			return 1 - (1 - t) * (1 - t);
		}

		static float EaseInOutQuad(float t)
		{
			return t < 0.5f
				? 2 * t * t
				: 1 - powf(-2 * t + 2, 2) / 2;
		}

		/// @}

		/// @name Cubic
		/// @{

		static float EaseInCubic(float t)
		{
			return t * t * t;
		}

		static float EaseOutCubic(float t)
		{
			return 1 - powf(1 - t, 3);
		}

		static float EaseInOutCubic(float t)
		{
			return t < 0.5f
				? 4 * t * t * t
				: 1 - powf(-2 * t + 2, 3) / 2;
		}

		/// @}

		/// @name Quart
		/// @{
		
		static float EaseInQuart(float t)
		{
			return t * t * t * t;
		}

		static float EaseOutQuart(float t)
		{
			return 1 - powf(1 - t, 4);
		}

		static float EaseInOutQuart(float t)
		{
			return t < 0.5f
				? 8 * t * t * t * t
				: 1 - powf(-2 * t + 2, 4) / 2;
		}

		/// @}

		/// @name Quint
		/// @{
		
		static float EaseInQuint(float t)
		{
			return t * t * t * t * t;
		}

		static float EaseOutQuint(float t)
		{
			return 1 - powf(1 - t, 5);
		}

		static float EaseInOutQuint(float t)
		{
			return t < 0.5f
				? 16 * t * t * t * t * t
				: 1 - powf(-2 * t + 2, 5) / 2;
		}

		/// @}

		/// @name Expo
		/// @{
		
		static float EaseInExpo(float t)
		{
			return t == 0 ? 0 : powf(2, 10 * t - 10);
		}

		static float EaseOutExpo(float t)
		{
			return t == 1 ? 1 : 1 - powf(2, -10 * t);
		}

		static float EaseInOutExpo(float t)
		{
			if (t == 0) return 0;
			if (t == 1) return 1;
			return t < 0.5f
				? powf(2, 20 * t - 10) / 2
				: (2 - powf(2, -20 * t + 10)) / 2;
		}

		/// @}

		/// @name Circ
		/// @{
		
		static float EaseInCirc(float t)
		{
			return 1 - sqrtf(1 - t * t);
		}

		static float EaseOutCirc(float t)
		{
			return sqrtf(1 - powf(t - 1, 2));
		}

		static float EaseInOutCirc(float t)
		{
			return t < 0.5f
				? (1 - sqrtf(1 - powf(2 * t, 2))) / 2
				: (sqrtf(1 - powf(-2 * t + 2, 2)) + 1) / 2;
		}

		/// @}

		/// @name Back
		/// @{
		
		static float EaseInBack(float t)
		{
			return C3 * t * t * t - C1 * t * t;
		}

		static float EaseOutBack(float t)
		{
			return 1 + C3 * powf(t - 1, 3) + C1 * powf(t - 1, 2);
		}

		static float EaseInOutBack(float t)
		{
			return t < 0.5f
				? (powf(2 * t, 2) * ((C2 + 1) * 2 * t - C2)) / 2
				: (powf(2 * t - 2, 2) * ((C2 + 1) * (2 * t - 2) + C2) + 2) / 2;
		}

		/// @}

		/// @name Elastic
		/// @{
		
		static float EaseInElastic(float t)
		{
			if (t == 0) return 0;
			if (t == 1) return 1;
			return -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75f) * C4);
		}

		static float EaseOutElastic(float t)
		{
			if (t == 0) return 0;
			if (t == 1) return 1;
			return powf(2, -10 * t) * sinf((t * 10 - 0.75f) * C4) + 1;
		}

		static float EaseInOutElastic(float t)
		{
			if (t == 0) return 0;
			if (t == 1) return 1;
			return t < 0.5f
				? -(powf(2, 20 * t - 10) * sinf((20 * t - 11.125f) * C5)) / 2
				: (powf(2, -20 * t + 10) * sinf((20 * t - 11.125f) * C5)) / 2 + 1;
		}

		/// @}

		/// @name Bounce
		/// @{
		
		static float EaseOutBounce(float t)
		{
			const float n1 = 7.5625f;
			const float d1 = 2.75f;

			if (t < 1 / d1)
				return n1 * t * t;
			else if (t < 2 / d1)
			{
				t -= 1.5f / d1;
				return n1 * t * t + 0.75f;
			}
			else if (t < 2.5f / d1)
			{
				t -= 2.25f / d1;
				return n1 * t * t + 0.9375f;
			}
			else
			{
				t -= 2.625f / d1;
				return n1 * t * t + 0.984375f;
			}
		}

		static float EaseInBounce(float t)
		{
			return 1 - EaseOutBounce(1 - t);
		}

		static float EaseInOutBounce(float t)
		{
			return t < 0.5f
				? (1 - EaseOutBounce(1 - 2 * t)) / 2
				: (1 + EaseOutBounce(2 * t - 1)) / 2;
		}

		/// @}
	};
}	// namespace RHEngine
