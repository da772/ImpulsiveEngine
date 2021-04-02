
newoption {
	trigger = "target-name",
	value = "ImpulseEditor",
	description = "set target name"
}
if not _OPTIONS["target-name"] then
	_OPTIONS["target-name"] = "ImpulseEditor"
end
targetName = _OPTIONS["target-name"]

workspace(targetName)
	architecture "x64"

	android_version = 21

	startproject(targetName)

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

	filter "system:windows"
		makesettings [[
			CC = clang
			CXX = clang++
		]]

	filter "system:linux"
		makesettings [[
			CC = clang
			CXX = clang++
		]]
			
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


include "ImpulseEngine"
include "ImpulseEditor/ImpulseEditor/Scripts/CPP"


project (targetName)
	location "ImpulseEditor"
	kind "ConsoleApp"
	language "C++"
		cppdialect "C++17"
	targetdir ("%{prj.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.location}/Bin-Obj/" .. outputdir .. "/%{prj.name}")
	if _OPTIONS['hot-reload'] then
	staticruntime "off"
	defines 
	{
		"GE_HOT_RELOAD"
	}
	else
	staticruntime "on"
	end

	defines 
	{
		"GE_APP_NAME=\""..targetName.."\"",
	}
	
	files 
	{
		"%{prj.location}/ImpulseEditor/Source/**.h",
		"%{prj.location}/ImpulseEditor/Source/**.c",
		"%{prj.location}/ImpulseEditor/Source/**.cpp",
		"%{prj.location}/ImpulseEditor/Source/**.hpp"
	}

	removefiles
	{
		"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.cpp",
		"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.c",
		"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.h",
		"%{prj.location}/ImpulseEditor/Source/Engine/Android/**.cpp",
		"%{prj.location}/ImpulseEditor/Source/Engine/Android/**.c",
		"%{prj.location}/ImpulseEditor/Source/Engine/Android/**.h"
	}

	includedirs 
	{
		"ImpulseEngine/ImpulseEngine/vendor/spdlog/include",
		"ImpulseEngine/ImpulseEngine/Source",
		"ImpulseEngine/ImpulseEngine/vendor",
		"ImpulseEngine/%{IncludeDir.glm}",
		"ImpulseEngine/%{IncludeDir.entt}",	
		"ImpulseEngine/%{IncludeDir.cr}",
		"ImpulseEngine/%{IncludeDir.vector}",
		"ImpulseEngine/%{IncludeDir.reflection}",
		"%{prj.location}/ImpulseEditor/Source/",
		"%{prj.location}/ImpulseEditor/include/",
		"%{prj.location}/ImpulseEditor/Scripts/CPP/Generated"

	}

	libdirs
	{
		"ImpulseEngine/%{IncludeDir.Vulkan}/lib",
		
	}
	if _OPTIONS["hot-reload"] then
		libdirs
		{
			"ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"
		}
	else
		libdirs
		{
			"ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"
		}
	end


	links
	{
		"ImpulseEngine"
	}

	if _OPTIONS["hot-reload"] then
	defines
	{
		"GE_DYNAMIC_LINK",
		"GE_PRJ_OFFSET=3",
		"GE_EDITOR"
	}
	else
	defines 
	{
		"GE_PRJ_OFFSET=3"
	}
	links
	{
		"Scripts_CPP"
	}
	end
	if _OPTIONS['server'] then
		defines 
		{
			"GE_SERVER_APP"	
		}
	end

	filter "platforms:x86_64"
		defines 
		{
			"BUILD_ARCHITECTURE=\"x86_64\""
		}

	filter "system:windows"
		systemversion "latest"
		kind "WindowedApp"

		excludes 
		{ 
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**" 
		}

		defines
		{
			"GE_PLATFORM_WINDOWS",
			"MS_BUILD_BIN=\"$(MSBuildBinPath)\""
		}
		postbuildcommands
		{
			"XCOPY /I /E /S /Y \"$(ProjectDir)ImpulseEditor/Data\" \"$(TargetDir)Data\""
		}
		if _OPTIONS["hot-reload"] then
		postbuildcommands
		{
			"copy /Y  \"%{wks.location}ImpulseEngine\\ImpulseEngine\\Bin\\".. outputdir.."\\ImpulseEngine\\ImpulseEngine.dll\" \"$(TARGETDIR)ImpulseEngine.dll\""
		}
		end


		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"
			kind "ConsoleApp"
		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
			kind "ConsoleApp"
		filter "configurations:Dist"
			defines "GE_DIST"
			runtime "Release"
			optimize "On"
			kind "WindowedApp"


		filter "system:linux"
			linkgroups 'on'
			systemversion "latest"
			kind "ConsoleApp"
			if _OPTIONS["hot-reload"] then
				linkoptions {"-F ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"}
			else
				linkoptions {"-F ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"}
			end
			postbuildcommands
			{
			--	"cp -rf \"$(ProjectDir)ImpulseEditor/Data\" \"%{prj.location}/Bin/" .. outputdir .. "/ImpulseEditor/Data\\"""
			}

			links 
			{
				"dl",
				"pthread",
				"stdc++fs"
			}
			if _OPTIONS['server'] then
			else
			links 
			{
				"GL",
				"X11",
				"openal"
			}
			end

			excludes 
			{ 
				"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**" 
			}
	
			defines
			{
				"GE_PLATFORM_LINUX"
			}
	
			
	
			
			filter "configurations:Debug"
				defines "GE_DEBUG"
				runtime "Debug"
				symbols "On"
				kind "ConsoleApp"
				defines 
				{
					"BUILD_CONFIG=\"Debug\""
				}
			filter "configurations:Release"
				defines "GE_RELEASE"
				runtime "Release"
				optimize "On"
				defines 
				{
					"BUILD_CONFIG=\"Release\""
				}
			filter "configurations:Dist"
				defines "GE_DIST"
				runtime "Release"
				optimize "On"
				defines 
				{
					"BUILD_CONFIG=\"Dist\""
				}					

	filter "system:ios"
		architecture "ARM"
		kind "WindowedApp"
		linkoptions ("-F ../ImpulseEngine/%{IncludeDir.firebase}/lib/ios" .. " -ObjC")
		if _OPTIONS["hot-reload"] then
			linkoptions {"-F ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"}
		else
			linkoptions {"-F ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"}
		end
		defines
		{
			"GE_PLATFORM_IOS"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
			["ARCHS"] = "$(ARCHS_STANDARD)",
			["TARGETED_DEVICE_FAMILY"] = "1,2",
			["IPHONEOS_DEPLOYMENT_TARGET"] = "11.0",
			["ASSETCATALOG_COMPILER_APPICON_NAME"] = "AppIcon",
			["ONLY_ACTIVE_ARCH"] = "YES"
		}

		files 
		{
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.m",
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.mm",
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.c",
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.h",
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.storyboard",
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**.plist",
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/*.json",
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/iOSImages.xcassets",
		}
			
		includedirs 
		{
			"%{prj.location}/ImpulseEditor/Source",
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS",
	
			"ImpulseEngine/%{IncludeDir.firebase}/include"
		}

		links
		{
			"OpenGLES.framework",
			"GLKit.framework",
			"UIKit.framework",
			"OpenAL.framework",
			"GoogleMobileAds.framework",
			"firebase.framework",
			"firebase_analytics.framework",
			"firebase_admob.framework",
		}
		

		postbuildcommands
		{
			"cp -rf ${PROJECT_DIR}/ImpulseEditor/Data ${TARGET_BUILD_DIR}/%{prj.name}.app/",
			"cp -rf ${PROJECT_DIR}/ImpulseEditor/Source/iOS/GoogleService-Info.plist ${TARGET_BUILD_DIR}/%{prj.name}.app/"

		}


		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"
			defines 
            {
                "BUILD_CONFIG=\"Debug\""
            }
			
		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
			defines 
            {
                "BUILD_CONFIG=\"Release\""
            }
			
		filter "configurations:Dist"
			defines "GE_DIST"
			runtime "Release"
			optimize "On"
			defines
			{
                "BUILD_CONFIG=\"Dist\""
            }
	
	filter "system:android"
		architecture "ARM"
		toolset "clang"
		cppdialect "gnu++17"
		kind "SharedLib"
		linkoptions { "-lm"}
		rtti ("On")
		exceptionhandling ("On")

		if _OPTIONS["hot-reload"] then
			linkoptions {"-F ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"}
		else
			linkoptions {"-F ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"}
		end

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
			"OpenSLES",
			"firebase_app",
			"firebase_analytics",
			"firebase_admob"
		}

		files 
		{
			"%{prj.location}/ImpulseEditor/Source/Engine/Android/**.h",
			"%{prj.location}/ImpulseEditor/Source/Engine/Android/**.c",
			"%{prj.location}/ImpulseEditor/Source/Engine/Android/**.cpp",
		}

		includedirs 
		{
			"%{prj.location}/ImpulseEditor/Source",
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
			defines "GE_DIST"
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
			"mkdir \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\libImpulseEditor.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/Bin/" .. outputdir .. "/ImpulseEditor\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)ImpulseEditor/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
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
			"mkdir \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"copy /y \"%{wks.location}x64\\%{cfg.buildcfg}\\libImpulseEditor.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}x64\" \"%{prj.location}/Bin/" .. outputdir .. "/ImpulseEditor\"",
			"RMDIR /Q/S \"%{wks.location}x64\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)ImpulseEditor/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
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
			"mkdir \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"copy /y \"%{wks.location}ARM\\%{cfg.buildcfg}\\libImpulseEditor.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}ARM\" \"%{prj.location}/Bin/" .. outputdir .. "/ImpulseEditor\"",
			"RMDIR /Q/S \"%{wks.location}ARM\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)ImpulseEditor/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
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
			"mkdir \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"copy /y \"%{wks.location}ARM64\\%{cfg.buildcfg}\\libImpulseEditor.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}ARM64\" \"%{prj.location}/Bin/" .. outputdir .. "/ImpulseEditor\"",
			"RMDIR /Q/S \"%{wks.location}ARM64\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)ImpulseEditor/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}

	filter "system:macosx"
		systemversion "latest"
		kind "WindowedApp"
		buildoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}
		if _OPTIONS["hot-reload"] then
			linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib", "-F ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"}
		else
			linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib", "-F ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"}
		end
		defines
		{
			"GE_PLATFORM_MACOSX"
		}

		excludes 
		{ 
			"%{prj.location}/ImpulseEditor/Source/Engine/iOS/**" 
		}

		includedirs
		{
	
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
			"OpenAL.framework",
			"vulkan.1.1.130"

		}

		postbuildcommands
		{
			"cp -rf ${PROJECT_DIR}/ImpulseEditor/\"Data\" ${TARGET_BUILD_DIR}/%{prj.name}.app/Contents/MacOS",
			--"cp -rf ${PROJECT_DIR}/ImpulseEditor/\"Res\" ${TARGET_BUILD_DIR}",
		}

		
		filter "configurations:Debug"
			defines "GE_DEBUG"
			runtime "Debug"
			symbols "On"
			defines 
            {
                "BUILD_CONFIG=\"Debug\""
            }
		filter "configurations:Release"
			defines "GE_RELEASE"
			runtime "Release"
			optimize "On"
			defines 
            {
                "BUILD_CONFIG=\"Release\""
            }
		filter "configurations:Dist"
			defines "GE_DIST"
			runtime "Release"
			optimize "On"
			defines 
            {
                "BUILD_CONFIG=\"Dist\""
            }
				

