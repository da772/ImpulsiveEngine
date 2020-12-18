#pragma once
#include "GEngine.h"


using namespace GEngine;

struct FPlatformInfo {

	Vector2f pos;
	Vector2f scale;
	Vector2f velocity;
	float maxHeight;
	int gridStartX, gridEndX, gridStartY, gridEndY;
	Vector2f worldPos;

};


class ProceduralPlatformGeneration {

public:
	static std::vector<Vector4f> GenerateLevel(uint32_t seed, const Vector2f& startPos, const Vector2f& endPos, const Vector2f& minScale, const Vector2f& maxScale, const Vector2f& minVelocity, const Vector2f& maxVelocity, const Vector2f& playerScale, std::function< Vector2f(float,float)> getVel, float platformSizeMultiple = 0);

private:
	static Vector4f CalculatePosition(const Vector2f& startPos, const Vector2f& endPos, const Vector2f& minScale, const Vector2f& maxScale, const Vector2f& minJumpVelocity, const Vector2f& maxJumpVelocity, float platformSizeMultiple, std::vector<FPlatformInfo>& platforms,std::function< Vector2f(float, float)> getVel, const Vector2f& playerScale, int** grid, int* done);
	

	
};
