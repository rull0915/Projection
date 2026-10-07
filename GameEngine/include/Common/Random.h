#include <random>
#include <mutex>
#include <array>

namespace REngine
{
	namespace Random
	{
		// スレッドセーフにするためのmutex
		inline std::mutex mtx;

		inline std::mt19937_64 mt;

		inline bool Init()
		{
			// 擬似乱数生成器の状態シーケンスのサイズ分、
			// シードを用意する
			std::array<
				std::seed_seq::result_type,
				std::mt19937::state_size
			> seed_data;

			// 非決定的な乱数でシード列を構築する
			std::random_device rd;
			std::generate(seed_data.begin(), seed_data.end(), std::ref(rd));

			std::seed_seq seq(seed_data.begin(), seed_data.end());

			// 擬似乱数生成器をシード列で初期化
			mt.seed(seq);

			return true;
		}

		// 数値型のみ指定可能のtemplate関数
		template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
		inline T Get(T min, T max)
		{
			// 初回呼び出し時に確実に初期化を行う
			static bool init = Init();

			// ロック
			std::lock_guard lock(mtx);

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
