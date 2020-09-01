#pragma once


namespace GEngine {




	class Utility {
	public:
		static std::string IPV4ToString(uint32_t ip);
		static glm::vec3 ScreenPosToWorldPos(glm::vec2 pos);
		static std::string GenerateHash(const int len);
		static void GenerateHash(char* str, const int len);


	};
	

}