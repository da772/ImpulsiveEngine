#pragma once

namespace GEngine {

	class GEMath {
	public:
		static float GE_API MapRange(float x, float min1, float max1, float min2, float max2);
        static int GE_API sign(int x);
        static int GE_API sign(float x);
		static float GE_API lerp(float a, float b, float amt);
		static Vector3f GE_API lerp(Vector3f a, Vector3f b, float amt);
		static Vector2f GE_API lerp(Vector2f a, Vector2f b, float amt);
		static float GE_API distance(Vector3f a, Vector3f b);
		static float GE_API distance(Vector2f a, Vector2f b);
		static float GE_API max(float in, float max);
		static float GE_API clamp(float in, float min, float max);
		static int GE_API clamp(int in, int min, int max);
		static Vector2f GE_API projection(const Vector2f& a, const Vector2f& b);
		static float GE_API dot(const Vector2f& a, const Vector2f& b);
		static float GE_API magnitude(const Vector2f a);
		static Vector2f GE_API normalize(const Vector2f& v);
		static Vector2f GE_API reflect(const Vector2f& v, const Vector2f n);
		static float GE_API ClosestMultiple(float x, float n);
		static float GE_API RadToDeg(float r);
		
	};


}
