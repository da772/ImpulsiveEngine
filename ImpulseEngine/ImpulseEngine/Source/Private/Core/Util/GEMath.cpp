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

	Vector3f GEMath::lerp(Vector3f a, Vector3f b, float amt)
	{
		return { lerp(a.x, b.x, amt), lerp(a.y, b.y, amt), lerp(a.z,b.z,amt) };
	}

	Vector2f GEMath::lerp(Vector2f a, Vector2f b, float amt)
	{
		return { lerp(a.x, b.x, amt), lerp(a.y, b.y, amt)};
	}

	float GEMath::distance(Vector3f a, Vector3f b)
	{
		return sqrt(pow(a.x - b.x, 2.f) + pow(a.y - b.y, 2.f) + pow(a.z - b.z, 2.f));
	}

	float GEMath::distance(Vector2f a, Vector2f b)
	{
		return sqrt(pow(a.x - b.x, 2.f) + pow(a.y - b.y, 2.f));
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

	Vector2f GEMath::projection(const Vector2f& a, const Vector2f& normal)
	{
		return (normal * dot(a, normal));
	}

	float GEMath::dot(const Vector2f& a, const Vector2f& b)
	{
		return a.x* b.x + a.y * b.y;
	}

	float GEMath::magnitude(const Vector2f a)
	{
		return (sqrt(a.x * a.x + a.y * a.y));
	}

	Vector2f GEMath::normalize(const Vector2f& v)
	{
		return v / magnitude(v);
	}

	Vector2f GEMath::reflect(const Vector2f& v, const Vector2f n)
	{
		return v - 2 * (dot(v, n)) * n;
	}

	float GEMath::ClosestMultiple(float x, float n)
	{
		float f = x * 2.f;
		float d = x - floor(x);

		if (d >= n / 2.f) {
			return ceil(f) * n;
		}
		else {
			return floor(f) * n;
		}

	}

}
