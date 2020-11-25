
newoption {
	trigger = "build-openal",
	description = "builds open al"
}

IncludeDir = {}
IncludeDir["GLFW"] = "ImpulseEngine/vendor/GLFW/include"
IncludeDir["ENET"] = "ImpulseEngine/vendor/Enet/include"
IncludeDir["Glad"] = "ImpulseEngine/vendor/Glad/include"
IncludeDir["Vulkan"] = "ImpulseEngine/vendor/Vulkan"
IncludeDir["ImGui"] = "ImpulseEngine/vendor/imgui"
IncludeDir["glm"] = "ImpulseEngine/vendor/glm"
IncludeDir["stb_image"] = "ImpulseEngine/vendor/stbimage"
IncludeDir["gltext"] = "ImpulseEngine/vendor/glText/include"
IncludeDir["httplib"] = "ImpulseEngine/vendor/httplib/include"
IncludeDir["miniupnpc"] = "ImpulseEngine/vendor/miniupnpc/include"
IncludeDir["freetype"] = "ImpulseEngine/vendor/freetype-2.10.0/include"
IncludeDir["freetypegl"] = "ImpulseEngine/vendor/freetype-2.10.0/include/freetype-gl"
IncludeDir["entt"] = "ImpulseEngine/vendor/entt/include"
IncludeDir["firebase"] = "ImpulseEngine/vendor/firebase"
IncludeDir["box2d"] = "ImpulseEngine/vendor/box2d/include"
IncludeDir["OpenAL"] = "ImpulseEngine/vendor/OpenAL/include"
IncludeDir["Vorbis"] = "ImpulseEngine/vendor/Vorbis/include"
IncludeDir["zlib"] = "ImpulseEngine/vendor/zlib/include"


group "Dependencies"
	include "ImpulseEngine/ImpulseEngine/vendor/GLFW"
	include "ImpulseEngine/ImpulseEngine/vendor/Glad"
	include "ImpulseEngine/ImpulseEngine/vendor/imgui"
	include "ImpulseEngine/ImpulseEngine/vendor/Enet"
	include "ImpulseEngine/ImpulseEngine/vendor/miniupnpc"
	include "ImpulseEngine/ImpulseEngine/vendor/zlib"
	include "ImpulseEngine/ImpulseEngine/vendor/freetype-2.10.0"
	--include "ImpulseEngine/ImpulseEngine/vendor/freetype-gl"
	include "ImpulseEngine/ImpulseEngine/vendor/box2d"
	if _OPTIONS['build-openal'] then
		include "ImpulseEngine/ImpulseEngine/vendor/OpenAL"
	end
	include "ImpulseEngine/ImpulseEngine/vendor/Vorbis"
	

group ""

project "ImpulseEngine"
	location "ImpulseEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


	targetdir ("%{prj.name}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.name}/Bin-Obj/" .. outputdir .. "/%{prj.name}")


	files 
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stbImage/**.h",
		"%{prj.name}/vendor/stbImage/**.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GE_ENGINE"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/Source/Public",
		"%{prj.name}/Source",
		"%{prj.name}/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Vulkan}/include",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.ENET}",
		"%{IncludeDir.gltext}",
		"%{IncludeDir.httplib}",
		"%{IncludeDir.miniupnpc}",
		"%{IncludeDir.freetypegl}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.firebase}/include",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.OpenAL}",
		"%{IncludeDir.Vorbis}",
		"%{IncludeDir.zlib}",

	}

	libdirs 
	{
		"%{IncludeDir.Vulkan}/lib"
	}
	
	

	links
	{
		"ImGui",
		"Enet",
		"miniupnpc",
		"zlib",
		"freetype",
		"box2d",
		"Vorbis",
		
	}
	if _OPTIONS['build-openal'] then	
	links
	{
		"OpenAL"	
	}
	end

	filter "system:macosx"
		systemversion "latest"

		pchheader "Source/gepch.h"
		pchsource "Source/gepch.cpp"
		
		buildoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}
		linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}

		files
		{
		}

		defines
		{
			"GE_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"GE_PLATFORM_MACOSX",
			"GE_WINDOW_GLFW",
			"GL_WITH_GLAD"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES" ,
			["SKIP_INSTALL"] = "YES"
		}

		includedirs
		{
		}

		links 
		{
			"Cocoa.framework",
			"OpenGL.framework",
			"IOKit.framework",
			"CoreVideo.framework",
			"libvulkan.1.1.130",
			"OpenAL.framework",
			"Glad",
			"GLFW"
		}

		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
		
		filter "configurations:Dist"
			defines "GE_DIST"
			runtime "Release"
			optimize "On"
		
	filter "system:ios"
		architecture "ARM"

		pchheader "Source/gepch.h"
		pchsource "Source/gepch.cpp"
		
		buildoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.firebase}/lib/ios"}
		linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.firebase}/lib/ios"}

		files
		{

		}
		defines
		{
			"GE_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"GE_PLATFORM_IOS"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
			["ARCHS"] = "$(ARCHS_STANDARD)",
			["SKIP_INSTALL"] = "YES"
		}

		includedirs
		{
			"ImpulseEngine/vendor/iOS/CoreFoundation/include",
		}

		links 
		{
			"OpenGLES.framework",
			"GLKit.framework",
			"UIKit.framework",
			"OpenAL.framework"
		}

		libdirs
		{
			"%{IncludeDir.firebase}/lib/ios"
		}

		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"

		

		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
		
		filter "configurations:Dist"
			defines "GE_DIST"
			runtime "Release"
			optimize "On"

	filter "system:android"
		toolset "clang"
		--kind "SharedLib"
		rtti ("On")
		cppdialect "gnu++17"
		linkoptions { "-lm" }
		exceptionhandling ("On")
		androidapilevel(android_version)

		pchheader "Source/gepch.h"
		pchsource "ImpulseEngine/Source/gepch.cpp"

		links 
		{
			"android",
			"EGL",
			"GLESv3",
			"OpenSLES",
			"firebase_app",
			"firebase_analytics",
			"firebase_admob"
		}
		
		
		filter "platforms:x86"
			architecture "x86"
			system "android"
			defines
			{
				"GE_BUILD_DLL",
				"GLFW_INCLUDE_NONE",
				"GE_PLATFORM_ANDROID"
			}
			libdirs
			{
				"%{IncludeDir.firebase}/lib/android/libx86"
			}

		filter "platforms:x64"
			architecture "x64"
			system "android"
			defines
			{
				"GE_BUILD_DLL",
				"GLFW_INCLUDE_NONE",
				"GE_PLATFORM_ANDROID"
			}
			libdirs
			{
				"%{IncludeDir.firebase}/lib/android/libx64"
			}

		filter "platforms:ARM"
			architecture "ARM"
			system "android"
			defines
			{
				"GE_BUILD_DLL",
				"GLFW_INCLUDE_NONE",
				"GE_PLATFORM_ANDROID"
			}
			libdirs
			{
				"%{IncludeDir.firebase}/lib/android/libarm"
			}

		filter "platforms:ARM64"
			architecture "ARM64"
			system "android"
			defines
			{
				"GE_BUILD_DLL",
				"GLFW_INCLUDE_NONE",
				"GE_PLATFORM_ANDROID"
			}
			libdirs
			{
				"%{IncludeDir.firebase}/lib/android/libarm64"
			}
			


		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
		
		filter "configurations:Dist"
			defines "GE_DIST"
			runtime "Release"
			optimize "On"
		



	filter "system:windows"
		systemversion "latest"
		pchheader "gepch.h"
		pchsource "ImpulseEngine/Source/gepch.cpp"

		files
		{
		}
		
		
		includedirs
		{
		}

		defines
		{
			"GE_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"GE_WINDOW_GLFW",
			"GE_PLATFORM_WINDOWS",
			"GL_WITH_GLAD"
		}
		

		links 
		{
			"opengl32.lib",
			"vulkan-1.lib",
			"Glad",
			"GLFW",
		}

		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"
		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
		filter "configurations:Dist"
			defines "GE_DIST"
			runtime "Release"
			optimize "On"
		filter { "system:windows", "action:gmake2" }
			cppdialect "gnu++17"
			defines
			{
				"GE_MINGW_",
				"GE_BUILD_DLL",
				"GLFW_INCLUDE_NONE",
				"GE_WINDOW_GLFW",
				"GE_PLATFORM_WINDOWS",
				"GL_WITH_GLAD"
			}
			links
			{
				"openal"
			}

	filter "system:linux"
		linkgroups 'on'
		systemversion "latest"
		pchheader "gepch.h"
		pchsource "ImpulseEngine/Source/gepch.cpp"
		cppdialect "gnu++17"
		files
		{
		}
		
		
		includedirs
		{
		}

		defines
		{
			"GLFW_INCLUDE_NONE",
			"GE_WINDOW_GLFW",
			"GE_PLATFORM_LINUX",
			"GL_WITH_GLAD"
		}
		

		links 
		{
			"GL",
			"Glad",
			"GLFW",
			"X11"
		}

		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"
		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
		filter "configurations:Dist"
			defines "GE_DIST"
			runtime "Release"
			optimize "On"

	
