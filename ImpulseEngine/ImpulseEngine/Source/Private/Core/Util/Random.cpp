#include "gepch.h"
#include "Public/Core/Util/Random.h"
#include <random>

namespace GEngine {

	std::mt19937 Random::s_Random;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Dist;
	bool Random::s_bInit = false;;

}
