#pragma once
#include "GEngine.h"


struct FPlatformInfo {

	glm::vec2 pos;
	glm::vec2 scale;
	glm::vec2 velocity;
	float maxHeight;

};


class ProceduralPlatformGeneration {

public:
	static std::vector<glm::vec4> GenerateLevel(uint32_t seed, const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& minScale, const glm::vec2& maxScale, const glm::vec2& minVelocity, const glm::vec2& maxVelocity, const glm::vec2& playerScale, std::function< glm::vec2(float,float)> getVel, float platformSizeMultiple = 0);

private:
	static glm::vec4 CalculatePosition(const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& minScale, const glm::vec2& maxScale, const glm::vec2& minJumpVelocity, const glm::vec2& maxJumpVelocity, float platformSizeMultiple, std::vector<FPlatformInfo>& platforms,std::function< glm::vec2(float, float)> getVel, const glm::vec2& playerScale, int** grid, bool* done);
	

	
};
