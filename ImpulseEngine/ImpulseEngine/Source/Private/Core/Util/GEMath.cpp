#include "gepch.h"
#include "Public/Core/Util/GEMath.h"

namespace GEngine {


	float GEMath::MapRange(float x, float min1, float max1, float min2, float max2)
	{
		return (x - min1) / (max1 - min1) * (max2 - min2) + min2;
	}

    int GEMath::sign(float x) {
        return x > 0 ? 1 : x == 0 ? 0 : -1;
    }

    int GEMath::sign(int x) {
       return x > 0 ? 1 : x == 0 ? 0 : -1;
   }

	float GEMath::lerp(float a, float b, float amt)
	{
		return a + amt * (b - a);
	}

	glm::vec3 GEMath::lerp(glm::vec3 a, glm::vec3 b, float amt)
	{
		return { lerp(a.x, b.x, amt), lerp(a.y, b.y, amt), lerp(a.z,b.z,amt) };
	}

	glm::vec2 GEMath::lerp(glm::vec2 a, glm::vec2 b, float amt)
	{
		return { lerp(a.x, b.x, amt), lerp(a.y, b.y, amt)};
	}

	float GEMath::distance(glm::vec3 a, glm::vec3 b)
	{
		return sqrt(pow(a.x - b.x, 2.f) + pow(a.y - b.y, 2.f) + pow(a.z - b.z, 2.f));
	}

	float GEMath::max(float in, float max)
	{
		return in > max ? in : max;
	}

	float GEMath::clamp(float in, float min, float max)
	{
		return in > max ? max : in < min ? min : in;
	}

	int GEMath::clamp(int in, int min, int max)
	{
		return in > max ? max : in < min ? min : in;
	}

	glm::vec2 GEMath::projection(const glm::vec2& a, const glm::vec2& normal)
	{
		return (normal * dot(a, normal));
	}

	float GEMath::dot(const glm::vec2& a, const glm::vec2& b)
	{
		return a.x* b.x + a.y * b.y;
	}

	float GEMath::magnitude(const glm::vec2 a)
	{
		return (sqrt(a.x * a.x + a.y * a.y));
	}

	glm::vec2 GEMath::normalize(const glm::vec2& v)
	{
		return v / magnitude(v);
	}

	glm::vec2 GEMath::reflect(const glm::vec2& v, const glm::vec2 n)
	{
		return v - 2 * (dot(v, n)) * n;
	}

}
