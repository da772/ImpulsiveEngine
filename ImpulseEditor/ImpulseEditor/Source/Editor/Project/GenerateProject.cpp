#include "GenerateProject.h"

using namespace Generation;

std::unordered_map <PlatformFlags, std::pair<std::string, std::string>> GenerateProject::platformFlagsStr = { {PlatformFlags::WINDOWS, {"Windows", "windows"}}, {PlatformFlags::UWP, {"Universal Windows Platform", "windows"}},{PlatformFlags::MACOSX, {"Mac OS X", "macosx"}},
		{PlatformFlags::LINUX, {"Linux", "linux"}}, {PlatformFlags::IOS, {"iOS", "ios"}}, {PlatformFlags::ANDROID, {"Android", "android"}}, {PlatformFlags::WASM, {"Web Assembly", "wasm"}} };

std::unordered_map <ProjectTypeFlags, std::pair<std::string, std::string>> GenerateProject::projectFlagsStr = { {ProjectTypeFlags::VISUAL_STUDIO_17, {"Visual Studio 17", "vs2017"}}, {ProjectTypeFlags::VISUAL_STUDIO_19, {"Visual Studio 19", "vs2019"}},
	{ProjectTypeFlags::XCODE_4, {"XCode", "xcode4"}}, {ProjectTypeFlags::MAKE, {"Make", "gmake2"}} };


std::unordered_map <GenerationFlags, std::pair<std::string, std::string>> GenerateProject::generationFlagsStr = { {GenerationFlags::SERVER, {"Server", "server"}}, {GenerationFlags::BUILD_EDITOR, {"Editor", "build-editor"}}, {GenerationFlags::BUILD_OPENAL, {"Build OpenAL", "build-openal"}},
		{GenerationFlags::BUILD_ENGINE, {"Build Engine", "build-engine"}}, {GenerationFlags::HOT_RELOAD, {"Hot Reload", "hot-reload"}}, {GenerationFlags::PACKAGE, {"Package", "package"}} };

std::unordered_map <PlatformFlags, PlatformStructure> GenerateProject::platformData = {
{PlatformFlags::WINDOWS,
{ {ProjectTypeFlags::VISUAL_STUDIO_19, ProjectTypeFlags::VISUAL_STUDIO_17, ProjectTypeFlags::MAKE},
	{GenerationFlags::BUILD_EDITOR, GenerationFlags::BUILD_ENGINE, GenerationFlags::BUILD_OPENAL, GenerationFlags::HOT_RELOAD, GenerationFlags::SERVER} }
},
{PlatformFlags::MACOSX,
{ {ProjectTypeFlags::XCODE_4, ProjectTypeFlags::MAKE}, {GenerationFlags::BUILD_EDITOR, GenerationFlags::BUILD_ENGINE, GenerationFlags::HOT_RELOAD, GenerationFlags::SERVER }}
},
{PlatformFlags::IOS,
{ {ProjectTypeFlags::XCODE_4}, { GenerationFlags::BUILD_ENGINE}}
},
{PlatformFlags::ANDROID,
{ {ProjectTypeFlags::MAKE, ProjectTypeFlags::VISUAL_STUDIO_19, ProjectTypeFlags::VISUAL_STUDIO_17}, { GenerationFlags::BUILD_ENGINE, GenerationFlags::BUILD_OPENAL}}
},
{PlatformFlags::LINUX,
{ {ProjectTypeFlags::MAKE},
	{GenerationFlags::BUILD_EDITOR, GenerationFlags::BUILD_ENGINE, GenerationFlags::BUILD_OPENAL, GenerationFlags::HOT_RELOAD, GenerationFlags::SERVER} }
}
};

std::string GenerateProject::GenerateCommand(const PlatformFlags& platform, const ProjectTypeFlags& project, const uint64_t& generation)
{
	std::string res = "";

	res += projectFlagsStr[project].second + " --os=" + platformFlagsStr[platform].second;

	for (uint64_t i = 0x01; i < (uint64_t)GenerationFlags::UNDEFINED; i = i << 1) {

		if ((uint64_t)generation & i) {
			res += " --" + generationFlagsStr[static_cast<GenerationFlags>(i)].second;
		}
	}


	return res;
}



uint64_t GenerateProject::GetDefaultGenerationFlags(const PlatformFlags& p)
{
	switch (p) {
	case PlatformFlags::WINDOWS:
		return ((uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::BUILD_OPENAL | (uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::HOT_RELOAD);
	case PlatformFlags::UWP:
		break;
	case PlatformFlags::MACOSX:
		return ((uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::HOT_RELOAD);
	case PlatformFlags::LINUX:
		return ((uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::HOT_RELOAD);
	case PlatformFlags::IOS:
		return ((uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::HOT_RELOAD);
	case PlatformFlags::ANDROID:
		return ((uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::BUILD_OPENAL | (uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::PACKAGE);
	case PlatformFlags::WASM:
		return ((uint64_t)GenerationFlags::BUILD_ENGINE | (uint64_t)GenerationFlags::BUILD_ENGINE);
	default:
	case PlatformFlags::UNDEFINED:
		return 0;
	}
}

ProjectTypeFlags GenerateProject::GetDefaultProjectType(const PlatformFlags& p)
{
	switch (p) {
	case PlatformFlags::WINDOWS:
		return ProjectTypeFlags::VISUAL_STUDIO_19;
	case PlatformFlags::UWP:
		return ProjectTypeFlags::VISUAL_STUDIO_19;
	case PlatformFlags::MACOSX:
		return ProjectTypeFlags::XCODE_4;
	case PlatformFlags::LINUX:
		return ProjectTypeFlags::MAKE;
	case PlatformFlags::IOS:
		return ProjectTypeFlags::XCODE_4;
	case PlatformFlags::ANDROID:
		return ProjectTypeFlags::VISUAL_STUDIO_19;
	case PlatformFlags::WASM:
		return ProjectTypeFlags::MAKE;
	default:
	case PlatformFlags::UNDEFINED:
		return ProjectTypeFlags::MAKE;
	}
}
