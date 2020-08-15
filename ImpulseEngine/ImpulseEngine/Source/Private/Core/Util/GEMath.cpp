#include "gepch.h"
#include "Public/Core/Util/GEMath.h"

namespace GEngine {


	float GEMath::MapRange(float x, float min1, float max1, float min2, float max2)
	{
		return (x - min1) / (max1 - min1) * (max2 - min2) + min2;
	}

}