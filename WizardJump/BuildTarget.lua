workspace "WizardJump"
	architecture "x64"

	android_version = 21
	startproject "WizardJump"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	
	}

	filter "system:android"
		platforms 
		{
			"ARM",
			"x86",
			"ARM64",
			"x64"
		}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "ImpulseEngine"

project "WizardJump"
	location "WizardJump"
	kind "ConsoleApp"
	language "C++"
			cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{prj.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.location}/Bin-Obj/" .. outputdir .. "/%{prj.name}")

	defines 
	{
		"GE_PROJECT_WizardJump"
	}
	
	files 
	{
		"%{prj.location}/%{prj.name}/Source/**.h",
		"%{prj.location}/%{prj.name}/Source/**.c",
		"%{prj.location}/%{prj.name}/Source/**.cpp"
	}

	removefiles
	{
		"%{prj.location}/%{prj.name}/Source/iOS/**.cpp",
		"%{prj.location}/%{prj.name}/Source/iOS/**.c",
		"%{prj.location}/%{prj.name}/Source/iOS/**.h",
		"%{prj.location}/%{prj.name}/Source/Android/**.cpp",
		"%{prj.location}/%{prj.name}/Source/Android/**.c",
		"%{prj.location}/%{prj.name}/Source/Android/**.h"
	}

	includedirs 
	{
		"ImpulseEngine/ImpulseEngine/vendor/spdlog/include",
		"ImpulseEngine/ImpulseEngine/Source",
		"ImpulseEngine/ImpulseEngine/vendor",
		"ImpulseEngine/%{IncludeDir.glm}",
		"ImpulseEngine/%{IncludeDir.entt}",	
	}

	libdirs 
	{
		"ImpulseEngine/%{IncludeDir.Vulkan}/lib"
	}

	links
	{
		"ImpulseEngine"
	}
	



	filter "system:windows"
		systemversion "latest"
		kind "WindowedApp"

		postbuildcommands
		{
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"$(TargetDir)Data\""
		}

		excludes 
		{ 
			"%{prj.location}/%{prj.name}/Source/iOS/**" 
		}


		includedirs
		{
			"ImpulseEngine/%{IncludeDir.duktape}/src/win32/"
		}

		defines
		{
			"GE_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"
			kind "ConsoleApp"
			
		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
		filter "configurations:Dist"
			defines "GE_Dist"
			runtime "Release"
			optimize "On"

	filter "system:ios"
		architecture "ARM"
		kind "WindowedApp"
		linkoptions (linkoptions ("-F ../ImpulseEngine/%{IncludeDir.firebase}/lib/ios" .. " -ObjC"))
		
		defines
		{
			"GE_PLATFORM_IOS"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
			["ARCHS"] = "$(ARCHS_STANDARD)",
			["TARGETED_DEVICE_FAMILY"] = "1,2",
			["ASSETCATALOG_COMPILER_APPICON_NAME"] = "AppIcon",
		}

		files 
		{
			"%{prj.location}/%{prj.name}/Source/iOS/**.m",
			"%{prj.location}/%{prj.name}/Source/iOS/**.mm",
			"%{prj.location}/%{prj.name}/Source/iOS/**.c",
			"%{prj.location}/%{prj.name}/Source/iOS/**.h",
			"%{prj.location}/%{prj.name}/Source/iOS/**.storyboard",
			"%{prj.location}/%{prj.name}/Source/iOS/**.plist",
			"%{prj.location}/%{prj.name}/Source/iOS/*.json",
			"%{prj.location}/%{prj.name}/Source/iOS/iOSImages.xcassets",
		}
			
		includedirs 
		{
			"%{prj.location}/%{prj.name}/Source",
			"%{prj.location}/%{prj.name}/Source/iOS",
			"ImpulseEngine/%{IncludeDir.duktape}/src/unix/",
			"ImpulseEngine/%{IncludeDir.firebase}/include"
		}

		links
		{
			"OpenGLES.framework",
			"GLKit.framework",
			"UIKit.framework",
			"GoogleMobileAds.framework",
			"firebase.framework",
			"firebase_analytics.framework",
			"firebase_admob.framework",
		}

		postbuildcommands
		{
			"cp -rf ${PROJECT_DIR}/%{prj.name}/\"Data\" ${TARGET_BUILD_DIR}/%{prj.name}.app/",
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
			defines "GE_Dist"
			runtime "Release"
			optimize "On"
	
	filter "system:android"
		toolset "clang"
		cppdialect "gnu++17"
		kind "SharedLib"
		linkoptions { "-lm"}
		rtti ("On")
		exceptionhandling ("On")

		androidapilevel(android_version)
		defines
		{
			"GE_PLATFORM_ANDROID"
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

		files 
		{
			"%{prj.location}/%{prj.name}/Source/Android/**.h",
			"%{prj.location}/%{prj.name}/Source/Android/**.c",
			"%{prj.location}/%{prj.name}/Source/Android/**.cpp",
			"ImpulseEngine/%{IncludeDir.duktape}/src/unix/"
		}

		includedirs 
		{
			"%{prj.location}/%{prj.name}/Source",
			"ImpulseEngine/%{IncludeDir.duktape}/src/unix"
		}
		androidLibDir = ""

		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
			
		filter "configurations:Dist"
			defines "GE_Dist"
			runtime "Release"
			optimize "On"

		filter "platforms:x86"
			architecture "x86"
			libdirs
			{
				"ImpulseEngine/%{IncludeDir.firebase}/lib/android/libx86"
			}
			androidLibDir = "x86"
			postbuildcommands
			{
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib%{prj.name}.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/Bin/" .. outputdir .. "/%{prj.name}\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}
		filter "platforms:x64"
			architecture "x64"
			libdirs
			{
				"ImpulseEngine/%{IncludeDir.firebase}/lib/android/libx64"
			}
			androidLibDir = "x86_64"
			postbuildcommands
			{
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib%{prj.name}.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/Bin/" .. outputdir .. "/%{prj.name}\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}
			
		filter "platforms:ARM"
			architecture "ARM"
			libdirs
			{
				"ImpulseEngine/%{IncludeDir.firebase}/lib/android/libarm"
			}
			androidLibDir = "armeabi-v7a"
			postbuildcommands
			{
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib%{prj.name}.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/Bin/" .. outputdir .. "/%{prj.name}\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}

		filter "platforms:ARM64"
			architecture "ARM64"
			libdirs
			{
				"ImpulseEngine/%{IncludeDir.firebase}/lib/android/libarm64"
			}
			androidLibDir = "arm64-v8a"
			postbuildcommands
			{
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib%{prj.name}.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/Bin/" .. outputdir .. "/%{prj.name}\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}

		
	

		


	filter "system:macosx"
		systemversion "latest"
		kind "WindowedApp"
		buildoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}
		linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}
		defines
		{
			"GE_PLATFORM_MACOSX"
		}

		excludes 
		{ 
			"%{prj.location}/%{prj.name}/Source/iOS/**" 
		}

		includedirs
		{
			"ImpulseEngine/%{IncludeDir.duktape}/src/unix/"
		}
		
		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES"
		}

		links
		{
			"Cocoa.framework",
			"OpenGL.framework",
			"IOKit.framework",
			"CoreVideo.framework",
			"vulkan.1.1.130"

		}

		postbuildcommands
		{
			"cp -rf ${PROJECT_DIR}/%{prj.name}/\"Data\" ${TARGET_BUILD_DIR}/%{prj.name}.app/Contents/MacOS",
			--"cp -rf ${PROJECT_DIR}/%{prj.name}/\"Res\" ${TARGET_BUILD_DIR}",
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
			defines "GE_Dist"
			runtime "Release"
			optimize "On"
				

