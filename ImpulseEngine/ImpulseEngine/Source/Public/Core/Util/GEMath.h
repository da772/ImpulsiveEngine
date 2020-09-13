#pragma once

namespace GEngine {

	class GEMath {
	public:
		static float MapRange(float x, float min1, float max1, float min2, float max2);
        static int sign(int x);
        static int sign(float x);
		static float lerp(float a, float b, float amt);
		static glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float amt);
		static glm::vec2 lerp(glm::vec2 a, glm::vec2 b, float amt);
		static float distance(glm::vec3 a, glm::vec3 b);
		static float distance(glm::vec2 a, glm::vec2 b);
		static float max(float in, float max);
		static float clamp(float in, float min, float max);
		static int clamp(int in, int min, int max);
		static glm::vec2 projection(const glm::vec2& a, const glm::vec2& b);
		static float dot(const glm::vec2& a, const glm::vec2& b);
		static float magnitude(const glm::vec2 a);
		static glm::vec2 normalize(const glm::vec2& v);
		static glm::vec2 reflect(const glm::vec2& v, const glm::vec2 n);
		static float ClosestMultiple(float x, float n);
		
	};


}
