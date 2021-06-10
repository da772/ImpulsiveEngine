#include "GenerateProject.h"

using namespace Generation;

std::unordered_map <PlatformFlags, std::pair<std::string, std::string>> GenerateProject::platformFlagsStr = { {PlatformFlags::WINDOWS, {"Windows", "windows"}}, {PlatformFlags::UWP, {"Universal Windows Platform", "windows"}},{PlatformFlags::MACOSX, {"Mac OS X", "macosx"}},
		{PlatformFlags::LINUX, {"Linux", "linux"}}, {PlatformFlags::IOS, {"iOS", "ios"}}, {PlatformFlags::ANDROID, {"Android", "android"}}, {PlatformFlags::WASM, {"Web Assembly", "wasm"}} };

std::unordered_map <ProjectFlags, std::pair<std::string, std::string>> GenerateProject::projectFlagsStr = { {ProjectFlags::VISUAL_STUDIO_17, {"Visual Studio 17", "vs17"}}, {ProjectFlags::VISUAL_STUDIO_19, {"Visual Studio 19", "vs19"}},
	{ProjectFlags::XCODE_4, {"XCode", "xcode4"}}, {ProjectFlags::MAKE, {"Make", "gmake2"}} };


std::unordered_map <GenerationFlags, std::pair<std::string, std::string>> GenerateProject::generationFlagsStr = { {GenerationFlags::SERVER, {"Server", "--server"}}, {GenerationFlags::BUILD_EDITOR, {"Editor", "--build-editor"}}, {GenerationFlags::BUILD_OPENAL, {"Build OpenAL", "--build-openal"}},
		{GenerationFlags::BUILD_ENGINE, {"Build Engine", "build-engine"}}, {GenerationFlags::HOT_RELOAD, {"Hot Reload", "--hot-reload"}} };

std::unordered_map <PlatformFlags, PlatformStructure> GenerateProject::platformData = {
{PlatformFlags::WINDOWS,
{ {ProjectFlags::VISUAL_STUDIO_19, ProjectFlags::VISUAL_STUDIO_17, ProjectFlags::MAKE},
	{GenerationFlags::BUILD_EDITOR, GenerationFlags::BUILD_ENGINE, GenerationFlags::BUILD_OPENAL, GenerationFlags::HOT_RELOAD, GenerationFlags::SERVER} }
},
{PlatformFlags::MACOSX,
{ {ProjectFlags::XCODE_4, ProjectFlags::MAKE}, {GenerationFlags::BUILD_EDITOR, GenerationFlags::BUILD_ENGINE,  GenerationFlags::HOT_RELOAD, GenerationFlags::SERVER }}
},
{PlatformFlags::IOS,
{ {ProjectFlags::XCODE_4}, { GenerationFlags::BUILD_ENGINE}}
},
{PlatformFlags::ANDROID,
{ {ProjectFlags::MAKE, ProjectFlags::VISUAL_STUDIO_19, ProjectFlags::VISUAL_STUDIO_17}, { GenerationFlags::BUILD_ENGINE}}
},
{PlatformFlags::LINUX,
{ {ProjectFlags::MAKE},
	{GenerationFlags::BUILD_EDITOR, GenerationFlags::BUILD_ENGINE, GenerationFlags::BUILD_OPENAL, GenerationFlags::HOT_RELOAD, GenerationFlags::SERVER} }
}
};