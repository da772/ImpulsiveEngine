#pragma once
#include <GEngine.h>

namespace Generation {

	enum class GenerationFlags : uint64_t {

		SERVER = 0x01,
		ENGINE_SRC = 0x02,
		BUILD_EDITOR = 0x04,
		BUILD_ENGINE = 0x08,
		HOT_RELOAD = 0x10,
		PACKAGE = 0x20,
		BUILD_OPENAL = 0x40,
		UNDEFINED

	};

	enum class PlatformFlags : uint32_t {

		WINDOWS = 0x01,
		UWP = 0x02,
		MACOSX = 0x04,
		LINUX = 0x08,
		IOS = 0x10,
		ANDROID = 0x20,
		WASM = 0x40,
		UNDEFINED

	};

	enum class ProjectFlags : uint32_t {
		
		VISUAL_STUDIO_19 = 0x01,
		VISUAL_STUDIO_17 = 0x02,
		XCODE_4 = 0x04,
		MAKE = 0x08,
		UNDEFINED

	};

	struct PlatformStructure {

		std::vector<ProjectFlags> projectFlags;
		std::vector<GenerationFlags> generationFlags;

	};

	class GenerateProject {

	public:
		static inline std::string ProjectFlagToStr(ProjectFlags f) {
			return projectFlagsStr[f].first;
		}

		static inline std::string ProjectFlagToFlag(ProjectFlags f) {
			return projectFlagsStr[f].second;
		}

		static inline std::string PlatformFlagToStr(PlatformFlags f) {
			return platformFlagsStr[f].first;
		}

		static inline std::string PlatformFlagToFlag(PlatformFlags f) {
			return platformFlagsStr[f].second;
		}

		static inline std::string GenerateFlagStr(GenerationFlags f) {
			return generationFlagsStr[f].first;
		}

		static inline std::string GenerateFlagtoFlag(GenerationFlags f) {
			return generationFlagsStr[f].second;
		}

		static inline std::unordered_map <PlatformFlags, struct PlatformStructure>& GetPlatformData() {
			return platformData;
		}


	private:
		static std::unordered_map <ProjectFlags, std::pair<std::string ,std::string>> projectFlagsStr;
		static std::unordered_map <PlatformFlags, std::pair<std::string, std::string>> platformFlagsStr;
		static std::unordered_map <GenerationFlags, std::pair<std::string, std::string>> generationFlagsStr;
		static std::unordered_map <PlatformFlags, PlatformStructure> platformData;

	};

}

