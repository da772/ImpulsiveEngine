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
		static float max(float in, float max);
		static float clamp(float in, float min, float max);
		static int clamp(int in, int min, int max);
	};


}
