#include "Generation/ProceduralPlatformGeneration.hpp"


std::vector<glm::vec4> ProceduralPlatformGeneration::GenerateLevel(uint32_t seed, const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& minScale, const glm::vec2& maxScale, const glm::vec2& minVelocity, const glm::vec2& maxVelocity, const glm::vec2& playerScale, std::function< glm::vec2(float, float)> getVel, float platformSizeMultiple)
{
	const int xDist = endPos.x - startPos.x;
	const int yDist = endPos.y - startPos.y;
	int** grid = new int*[xDist];

	for (int x = 0; x < xDist; x++) {
		grid[x] = new int[yDist];
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
	bool bdone = false;


	glm::vec4 start = CalculatePosition(startPos, endPos, minScale, maxScale, minVelocity, maxVelocity, platformSizeMultiple, platforms, getVel, playerScale, grid, &bdone);
	pos.push_back(start);


	while (!bdone) {
		glm::vec4 start = CalculatePosition(startPos, endPos, minScale, maxScale, minVelocity, maxVelocity, platformSizeMultiple, platforms, getVel, playerScale, grid, &bdone);
		pos.push_back(start);
	}


	for (int x = 0; x < xDist; x++) {
		delete[] grid[x];
	}
	delete[] grid;

	return pos;
}

glm::vec4 ProceduralPlatformGeneration::CalculatePosition(const glm::vec2& startPos, const glm::vec2& endPos, const glm::vec2& minScale, const glm::vec2& maxScale, const glm::vec2& minVel, const glm::vec2& maxVel, float platformSizeMultiple, std::vector<FPlatformInfo>& platforms, std::function< glm::vec2(float, float)> getVel, const glm::vec2& playerScale, int** grid, bool* done)
{
	FPlatformInfo platform;
	int xDistance = endPos.x - startPos.x;
	int yDistance = endPos.y - startPos.y;
	if (platforms.size() <= 0) {

		int dir = GEngine::Random::IntRange(0, 1) == 0 ? 1 : -1;
		glm::vec2 _dist = { GEngine::Random::FloatRange(-minVel.x, maxVel.x), GEngine::Random::FloatRange(minVel.y, maxVel.y) };
		glm::vec2 vel = getVel(_dist.x, _dist.y);
		
		glm::vec2 _startPos = { 0 , 0 };
		int height = GEngine::Physics::GetMaxHeight(_startPos, vel);
		int time = GEngine::Physics::GetMaxVelocityTime(vel);

		glm::vec2 _endPos = GEngine::Physics::GetTrajectoryPoint2D(_startPos, vel, time);

		glm::vec2 scale = { GEngine::GEMath::ClosestMultiple(GEngine::Random::FloatRange(minScale.x, maxScale.x), .5f), GEngine::GEMath::ClosestMultiple( GEngine::Random::FloatRange(minScale.y,maxScale.y), .5f) };

		if (_endPos.x + scale.x / 2.f > _startPos.x) {
			scale.x = GEngine::GEMath::ClosestMultiple(_endPos.x - _startPos.x, .5f);
			if (scale.x <= 0) {
				scale.x = 1;
			}
		}

		if (_endPos.y + scale.y/2.f > _startPos.y) {
			scale.y = GEngine::GEMath::ClosestMultiple(_endPos.y - _startPos.y,.5f);
			if (scale.y <= 0) {
				scale.y = 1;
			}
		}

		_startPos.x += scale.x/2.f;
		_startPos.y += scale.y/2.f;
		
		int _iS = GEngine::GEMath::ClosestMultiple(_startPos.x, .5f), _iE = GEngine::GEMath::ClosestMultiple(_endPos.x, .5f);
		int _jS = GEngine::GEMath::ClosestMultiple(_startPos.y, .5f), _jE = GEngine::GEMath::ClosestMultiple(height, .5f);
		_iS = GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_iS - scale.x / 2.f, .5f), startPos.x, endPos.x, 0, xDistance);
		_iE = GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_iE + scale.x / 2.f, .5f), startPos.x, endPos.x, 0, xDistance);

		_jS = GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_jS - scale.y / 2.f, .5f), startPos.y, endPos.y, 0, yDistance);
		_jE = GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_jE + scale.y / 2.f, .5f), startPos.y, endPos.y, 0, yDistance);


		for (int i = _iS; i < _iE; i++) {
			for (int j = _jS; j <_jE; j++) {
				grid[i][j] = 1;
			}
		}

		platform.maxHeight = height;
		platform.pos = _endPos;
		platform.scale = scale;
		platform.velocity = _dist;

		platforms.push_back(platform);
		
		return { GEngine::GEMath::MapRange(_iS, 0, xDistance, startPos.x, endPos.x), GEngine::GEMath::MapRange(_jS, 0, yDistance, startPos.y, endPos.y), scale.x, scale.y };
	}
	else {

		const FPlatformInfo& lastPlatform = platforms[platforms.size() - 1];

		int dir = GEngine::Random::IntRange(0, 1) == 0 ? 1 : -1;
		glm::vec2 _dist = { lastPlatform.velocity.x, lastPlatform.velocity.y};

		calcPlatform:
		{
			glm::vec2 vel = getVel(_dist.x, _dist.y);

			glm::vec2 _startPos = { lastPlatform.pos.x + lastPlatform.scale.x/2.f*dir, lastPlatform.pos.y + lastPlatform.scale.y/2.f };
			int height = GEngine::Physics::GetMaxHeight(_startPos, vel);
			int time = GEngine::Physics::GetMaxVelocityTime(vel);

			glm::vec2 _endPos = GEngine::Physics::GetTrajectoryPoint2D(_startPos, vel, time);

			glm::vec2 scale = { GEngine::GEMath::ClosestMultiple(GEngine::Random::FloatRange(minScale.x, maxScale.x), .5f), GEngine::GEMath::ClosestMultiple(GEngine::Random::FloatRange(minScale.y,maxScale.y), .5f) };

			if (_endPos.x + scale.x / 2.f > _startPos.x) {
				scale.x = GEngine::GEMath::ClosestMultiple(_endPos.x - _startPos.x, .5f);
				if (scale.x <= 0) {
					scale.x = 1;
				}
			}

			if (_endPos.y + scale.y / 2.f > _startPos.y) {
				scale.y = GEngine::GEMath::ClosestMultiple(_endPos.y - _startPos.y, .5f);
				if (scale.y <= 0) {
					scale.y = 1;
				}
			}

			_startPos.x += scale.x / 2.f;
			_startPos.y += scale.y / 2.f;

			int _iS = GEngine::GEMath::ClosestMultiple(_startPos.x, .5f), _iE = GEngine::GEMath::ClosestMultiple(_endPos.x, .5f);
			int _jS = GEngine::GEMath::ClosestMultiple(_startPos.y, .5f), _jE = GEngine::GEMath::ClosestMultiple(height, .5f);
			_iS = GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_iS - scale.x / 2.f, .5f), startPos.x, endPos.x, 0, xDistance);
			_iE = GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_iE + scale.x / 2.f, .5f), startPos.x, endPos.x, 0, xDistance);

			_jS = GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_jS - scale.y / 2.f, .5f), startPos.y, endPos.y, 0, yDistance);
			_jE = GEngine::GEMath::MapRange(GEngine::GEMath::ClosestMultiple(_jE + scale.y / 2.f, .5f), startPos.y, endPos.y, 0, yDistance);

			bool failed = false;
			for (int i = _iS; i < _iE; i++) {
				for (int j = _jS; j < _jE; j++) {
					if (grid[i][j] == 1) {
						failed = true;
						_dist.x += .01f;
						_dist.y += .1f;
						break;
					}
				}
			}

			if (failed)
				goto calcPlatform;

			for (int i = _iS; i < _iE; i++) {
				for (int j = _jS; j < _jE; j++) {
					grid[i][j] == 1;
				}
			}

			platform.maxHeight = height;
			platform.pos = _endPos;
			platform.scale = scale;
			platform.velocity = vel;

			platforms.push_back(platform);
			*done = true;

			return { GEngine::GEMath::MapRange(_iS, 0, xDistance, startPos.x, endPos.x), GEngine::GEMath::MapRange(_jS, 0, yDistance, startPos.y, endPos.y), scale.x, scale.y };
		}

	}



	platforms.push_back(platform);

	return { 5,5,1,1 };
}
