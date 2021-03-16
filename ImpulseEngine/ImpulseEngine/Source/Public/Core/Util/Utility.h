#pragma once


namespace GEngine {




	class Utility {
	public:
		static std::string IPV4ToString(uint32_t ip);
		static Vector3f ScreenPosToWorldPos(Vector2f pos);
		static std::string GenerateHash(const int len);
		static void GenerateHash(char* str, const int len);

	};
	

}