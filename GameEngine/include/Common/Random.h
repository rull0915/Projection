#include <random>

namespace REngine
{
	namespace Random
	{
		static std::mt19937_64 mt;

		static void Init()
		{
			// std::random_deviceを作成
			std::random_device rd;

			// 上記のrdで4つの値を使ってstd::seed_seqを作成
			std::seed_seq ss{ rd(), rd(), rd(), rd() };

			// 上記のss連続値を使って「mt」エンジンの再シードを行う（mt.seed()関数）
			mt.seed(ss);
		}

		// 数値型のみ指定可能のtemplate関数
		template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
		static T Get(T min, T max)
		{
			// 整数型の場合
			if constexpr (std::is_integral_v<T>)
			{
				std::uniform_int_distribution<T> dist(min, max);
				return dist(mt);
			}
			// 少数型の場合
			else
			{
				std::uniform_real_distribution<T> dist(min, max);
				return dist(mt);
			}
		}
	}
} // namespace REngine
