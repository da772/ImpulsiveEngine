
--include "Sandbox"
-- Include directories relative to root folder (solution dir)


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
IncludeDir["freetypegl"] = "ImpulseEngine/vendor/freetype-gl/src"
IncludeDir["freetype"] = "ImpulseEngine/vendor/freetype-2.10.0/include"
IncludeDir["duktape"] = "ImpulseEngine/vendor/duktape"
IncludeDir["entt"] = "ImpulseEngine/vendor/entt/include"
IncludeDir["firebase"] = "ImpulseEngine/vendor/firebase"
IncludeDir["box2d"] = "ImpulseEngine/vendor/box2d/include"

group "Dependencies"
	include "ImpulseEngine/ImpulseEngine/vendor/GLFW"
	include "ImpulseEngine/ImpulseEngine/vendor/Glad"
	include "ImpulseEngine/ImpulseEngine/vendor/imgui"
	include "ImpulseEngine/ImpulseEngine/vendor/Enet"
	include "ImpulseEngine/ImpulseEngine/vendor/miniupnpc"
	include "ImpulseEngine/ImpulseEngine/vendor/freetype-2.10.0"
	include "ImpulseEngine/ImpulseEngine/vendor/freetype-gl"
	include "ImpulseEngine/ImpulseEngine/vendor/box2d"

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
		"%{IncludeDir.duktape}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.firebase}/include",
		"%{IncludeDir.box2d}",



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
		"freetype-gl",
		"box2d"
	}

	filter "system:macosx"
		systemversion "latest"

		pchheader "Source/gepch.h"
		pchsource "Source/gepch.cpp"
		
		buildoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}
		linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}

		files
		{
			"%{IncludeDir.duktape}/src/unix/**.cpp",
			"%{IncludeDir.duktape}/src/unix/**.h"
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
			"%{IncludeDir.duktape}/src/unix"
		}

		links 
		{
			"Cocoa.framework",
			"OpenGL.framework",
			"IOKit.framework",
			"CoreVideo.framework",
			"libvulkan.1.1.130",
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
			"%{IncludeDir.duktape}/src/unix/**.cpp",
			"%{IncludeDir.duktape}/src/unix/**.h"
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
			"%{IncludeDir.duktape}/src/unix",
		}

		links 
		{
			"OpenGLES.framework",
			"GLKit.framework",
			"UIKit.framework"
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

		files
		{
			"%{IncludeDir.duktape}/src/unix/**.cpp",
			"%{IncludeDir.duktape}/src/unix/**.h"
		}
		
		includedirs
		{
			"%{IncludeDir.duktape}/src/unix",

		}
		links 
		{
			"android",
			"EGL",
			"GLESv3",
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
			"%{IncludeDir.duktape}/src/win32/**.h",
			"%{IncludeDir.duktape}/src/win32/**.cpp",
		}
		
		
		includedirs
		{
			"%{IncludeDir.duktape}/src/win32"
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
