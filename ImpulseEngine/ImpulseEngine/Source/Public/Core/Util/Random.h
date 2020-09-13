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

		inline static void SetSeed(uint32_t seed) {
			s_Random.seed(seed);
			s_bInit = true;
		}

		static int Int() {
			if (!s_bInit)
				Init();
			return (int)s_Dist(s_Random);
		}

		static int IntRange(int min, int max) {
			if (!s_bInit)
				Init();
			return min + ((int)s_Dist(s_Random) % (max - min + 1));
		}

		static float FloatRange(float min, float max) {
			if (!s_bInit)
				Init();
			return ((max - min) * ((float)s_Dist(s_Random) / (float)std::numeric_limits<uint32_t>::max()) + min);
		}



	private:
		static std::mt19937 s_Random;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Dist;
		static bool s_bInit;

	};


}
