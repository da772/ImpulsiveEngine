#pragma once


#include <random>

namespace GEngine {

	class Random
	{
	public:

		inline static void Init() {
			s_Random.seed(std::random_device()());
			s_bInit = true;
		}

		inline static void Init(int seed) {
			s_Random.seed(seed);
			s_bInit = true;
		}

		static inline float Float() {
			if (!s_bInit)
				Init();
			return (float)s_Dist(s_Random) / (float)std::numeric_limits<uint32_t>::max();
		}
	private:
		static std::mt19937 s_Random;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Dist;
		static bool s_bInit;

	};


}
