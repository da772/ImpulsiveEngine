#pragma once

namespace GEngine {

	class GEMath {
	public:
		static float MapRange(float x, float min1, float max1, float min2, float max2);
        static int sign(int x);
        static int sign(float x);
		static float lerp(float a, float b, float amt);
		static Vector3f lerp(Vector3f a, Vector3f b, float amt);
		static Vector2f lerp(Vector2f a, Vector2f b, float amt);
		static float distance(Vector3f a, Vector3f b);
		static float distance(Vector2f a, Vector2f b);
		static float max(float in, float max);
		static float clamp(float in, float min, float max);
		static int clamp(int in, int min, int max);
		static Vector2f projection(const Vector2f& a, const Vector2f& b);
		static float dot(const Vector2f& a, const Vector2f& b);
		static float magnitude(const Vector2f a);
		static Vector2f normalize(const Vector2f& v);
		static Vector2f reflect(const Vector2f& v, const Vector2f n);
		static float ClosestMultiple(float x, float n);
		
	};


}
