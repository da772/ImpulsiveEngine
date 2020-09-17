#include "Generation/ProceduralPlatformGeneration.hpp"

/*boo but fk u*/
#include "Character/CharacterController.hpp"


std::vector<glm::vec4> ProceduralPlatformGeneration::GenerateLevel(uint32_t seed, const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& minScale, const glm::vec2& maxScale, const glm::vec2& minVelocity, const glm::vec2& maxVelocity, const glm::vec2& playerScale, std::function< glm::vec2(float, float)> getVel, float platformSizeMultiple)
{
	const int xDist = endPos.x - startPos.x;
	const int yDist = endPos.y - startPos.y;
	int** grid = (int**)malloc(sizeof(int*) * xDist);

	for (int x = 0; x < xDist; x++) {
		grid[x] = (int*)malloc(sizeof(int) * yDist);
		memset(&grid[x][0], 0, sizeof(int) * yDist);
	}

	std::vector<FPlatformInfo> platforms;
	std::vector<glm::vec4> pos;
	GE_LOG_INFO("SEED {0}", seed);
	GEngine::Random::SetSeed(seed);

	float currentY = 0;
	float currentX = 0;

	float currentXScale = 1.f;
	float currentYScale = 1.f;
	int dir = 1;
	int bdone = 0;


	glm::vec4 start = CalculatePosition(startPos, endPos, minScale, maxScale, minVelocity, maxVelocity, platformSizeMultiple, platforms, getVel, playerScale, grid, &bdone);
	pos.push_back(start);


	while (bdone < 5) {
		glm::vec4 start = CalculatePosition(startPos, endPos, minScale, maxScale, minVelocity, maxVelocity, platformSizeMultiple, platforms, getVel, playerScale, grid, &bdone);
		pos.push_back(start);
	}



	for (int x = 0; x < xDist; x++) {
		free(grid[x]);
	}
	free(grid);

	return pos;
}

glm::vec4 ProceduralPlatformGeneration::CalculatePosition(const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& minScale, const glm::vec2& maxScale, const glm::vec2& minVel, const glm::vec2& maxVel, float platformSizeMultiple, std::vector<FPlatformInfo>& platforms, std::function< glm::vec2(float, float)> getVel, const glm::vec2& playerScale, int** grid, int* done)
{
	FPlatformInfo platform;
	int xDistance = endPos.x - startPos.x;
	int yDistance = endPos.y - startPos.y;
	if (platforms.size() <= 0) {

		int dir = GEngine::Random::IntRange(0, 1) == 0 ? 1 : -1;
		glm::vec2 _dist = { GEngine::Random::FloatRange(-minVel.x, maxVel.x), GEngine::Random::FloatRange(maxVel.y, maxVel.y) };
		glm::vec2 vel = getVel(_dist.x, _dist.y);

		glm::vec2 _startPos = { 0 , 0 };
		int height = GEngine::Physics::GetMaxHeight(_startPos, vel);
		int time = GEngine::Physics::GetMaxVelocityTime(vel);

		glm::vec2 _endPos = GEngine::Physics::GetTrajectoryPoint2D(_startPos, vel, time);

		glm::vec2 scale = { GEngine::GEMath::ClosestMultiple(GEngine::Random::FloatRange(minScale.x, maxScale.x), .5f), GEngine::GEMath::ClosestMultiple(GEngine::Random::FloatRange(minScale.y,maxScale.y), .5f) };

		if (_endPos.x + scale.x / 2.f > _startPos.x) {
			scale.x = GEMath::clamp(GEngine::GEMath::ClosestMultiple(_endPos.x - _startPos.x, .5f), minScale.x, maxScale.x);
			if (scale.x <= 0) {
				scale.x = 1;
			}

		}

		if (_endPos.y + scale.y / 2.f > _startPos.y) {
			scale.y = GEMath::clamp(GEngine::GEMath::ClosestMultiple(_endPos.y - _startPos.y, .5f), minScale.y, maxScale.y);
			if (scale.y <= 0) {
				scale.y = 1;
			}
		}

		if (_endPos.y - scale.y / 2.f < 0) {
			GEMath::clamp(GEngine::GEMath::ClosestMultiple(_endPos.x - startPos.y, .5f), minScale.y, maxScale.y);
		}



		int _iS = GEngine::GEMath::ClosestMultiple(_startPos.x, .5f), _iE = GEngine::GEMath::ClosestMultiple(_endPos.x, .5f);
		int _jS = GEngine::GEMath::ClosestMultiple(_startPos.y, .5f), _jE = GEngine::GEMath::ClosestMultiple(height, .5f);
		_iS = GEngine::GEMath::MapRange(_iS - scale.x / 2.f, startPos.x, endPos.x, 0, xDistance);
		_iE = GEMath::clamp(GEngine::GEMath::MapRange(_iE + scale.x / 2.f, startPos.x, endPos.x, 0, xDistance), 0.f, (float)xDistance-1);

		_jS = GEMath::clamp(GEMath::max(GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_jS - scale.y / 2.f, .5f), startPos.y, endPos.y, 0, yDistance), 0), 0.f, (float)yDistance-1);
		_jE = GEMath::clamp(GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_jE, .5f), startPos.y, endPos.y, 0, yDistance), 0.f, (float)yDistance-1);


		for (int i = _iS; i <= _iE; i++) {
			for (int j = _jS; j <= _jE; j++) {
				GE_LOG_DEBUG("Setting: {0},{1} to {2}", i, j, 1);
				grid[i][j] = 1;
			}
		}

		platform.maxHeight = height;
		platform.pos = _endPos;
		platform.scale = scale;
		platform.velocity = _dist;
		platform.gridStartX = _iS;
		platform.gridEndX = _iE;
		platform.gridStartY = _jS;
		platform.gridEndY = _jE;
		platform.worldPos = { GEngine::GEMath::MapRange(_iS, 0, xDistance, startPos.x, endPos.x), GEngine::GEMath::MapRange(_jS, 0, yDistance, startPos.y, endPos.y) };

		platforms.push_back(platform);

		return { platform.worldPos.x, platform.worldPos.y, scale.x, scale.y };
	}
	else {
		const FPlatformInfo& lastPlatform = platforms[platforms.size() - 1];

		
		glm::vec2 lastPos = { lastPlatform.pos.x, lastPlatform.pos.y + lastPlatform.scale.y / 2.f };
		glm::vec2 targetPos;
		glm::vec2 vel;
		int gridX, gridY;
		bool foundPos = false;
		for (int i = 0; i < xDistance; i++) {
			if (i == lastPlatform.pos.x) continue;
			for (int j = 0; j < yDistance; j++) {

				if (grid[i][j] != 0) continue;

				targetPos = { GEngine::GEMath::MapRange(i, 0, xDistance, startPos.x, endPos.x),GEngine::GEMath::MapRange(j, 0, yDistance, startPos.y, endPos.y) };
				vel = GEngine::Physics::GetVelocityToPosition(lastPos, targetPos);
				glm::vec2 requiredDrag = CharacterController::DragRequiredForVelocity(vel);
				GE_CORE_DEBUG("Checking {0},{1}, World Pos: {2},{3}, Vel: {4},{5}, Drag: {6}, {7}", i,j,targetPos.x, targetPos.y, vel.x, vel.y, requiredDrag.x, requiredDrag.y);
				if (requiredDrag.x > maxVel.x || requiredDrag.x < minVel.x || requiredDrag.y > maxVel.y || requiredDrag.y < minVel.y) continue;

				gridX = i;
				gridY = j;
				foundPos = true;
			}
		}
		*done += 1;
		if (!foundPos) { GE_CORE_ERROR("CANNOT FIND PLATFORM");  *done = 1000; return { 0,0,0, 0 }; }

		float height = GEngine::Physics::GetMaxHeight(targetPos, vel);;

		glm::vec2 scale = { 1,1 };

		int _iS = GEngine::GEMath::ClosestMultiple(lastPlatform.pos.x, .5f), _iE = GEngine::GEMath::ClosestMultiple(targetPos.x, .5f);
		int _jS = GEngine::GEMath::ClosestMultiple(targetPos.y, .5f), _jE = GEngine::GEMath::ClosestMultiple(height, .5f);
		_iS = GEngine::GEMath::MapRange(_iS - scale.x / 2.f, startPos.x, endPos.x, 0, xDistance);
		_iE = GEMath::clamp(GEngine::GEMath::MapRange(_iE + scale.x / 2.f, startPos.x, endPos.x, 0, xDistance), 0.f, (float)xDistance - 1);

		_jS = GEMath::clamp(GEMath::max(GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_jS - scale.y / 2.f, .5f), startPos.y, endPos.y, 0, yDistance), 0), 0.f, (float)yDistance - 1);
		_jE = GEMath::clamp(GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_jE, .5f), startPos.y, endPos.y, 0, yDistance), 0.f, (float)yDistance - 1);


		for (int i = _iS; i <= _iE; i++) {
			for (int j = _jS; j <= _jE; j++) {
				GE_LOG_DEBUG("Setting: {0},{1} to {2}", i, j, 1);
				grid[i][j] = 1;
			}
		}


		platform.maxHeight = height;
		platform.pos = targetPos;
		platform.scale = { 1,1 };
		platform.velocity = vel;
		platform.gridStartX = _iS;
		platform.gridEndX = _iE;
		platform.gridStartY = _jS;
		platform.gridEndY = _jE;
		platform.worldPos = targetPos;
		
		platforms.push_back(platform);

		return { platform.worldPos.x, platform.worldPos.y, 1,1 };

	}





	platforms.push_back(platform);

	return { 5,5,1,1 };
}
