
newoption {
	trigger = "target-name",
	value = "ImpulseEditor",
	description = "set target name"
}


newoption {
	trigger = "engine-source",
	value = "",
	description = "set location of engine source"
}


newoption {
	trigger = "build-editor",
	description = "build editor"
}

newoption {
	trigger = "package",
	description = "package project for distribution"
}

newoption {
	trigger = "build-engine",
	description = "build engine"
}

newoption {
	trigger = "hot-reload",
	description = "allows for hot reloading on the fly"
}



if not _OPTIONS["target-name"] then
	_OPTIONS["target-name"] = "ImpulseEditor"
end

targetName = _OPTIONS["target-name"]
if not _OPTIONS['engine-source'] then
	engineSrc = "%{wks.location}/"
	vendorSrc = ""
else 
	engineSrc = _OPTIONS["engine-source"]
	vendorSrc = _OPTIONS["engine-source"]
end


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

include "vendor"
if _OPTIONS['build-engine'] then
	if not _OPTIONS['engine-source'] then
		include ("ImpulseEngine")
	else 
		include (engineSrc.."ImpulseEngine")
	end
end
include(targetName.."/"..targetName.."/NativeScripts/")


project (targetName)
	location(targetName)
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

	if _OPTIONS['package'] then 
	defines 
	{
		"GE_PACKAGE"
	}
	end

	defines 
	{
		"GE_APP_NAME=\""..targetName.."\"",
	}
	
	files 
	{
		"%{prj.location}/"..targetName.."/Source/**.h",
		"%{prj.location}/"..targetName.."/Source/**.c",
		"%{prj.location}/"..targetName.."/Source/**.cpp",
		"%{prj.location}/"..targetName.."/Source/**.hpp"
	}

	removefiles
	{
		"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.cpp",
		"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.c",
		"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.h",
		"%{prj.location}/"..targetName.."/Source/Shared/Engine/Android/**.cpp",
		"%{prj.location}/"..targetName.."/Source/Shared/Engine/Android/**.c",
		"%{prj.location}/"..targetName.."/Source/Shared/Engine/Android/**.h"
	}

	includedirs 
	{
		engineSrc.."ImpulseEngine/ImpulseEngine/Source",
		engineSrc.."ImpulseEngine/ImpulseEngine/vendor",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.vector}",
		"%{IncludeDir.reflection}",
		"%{prj.location}/"..targetName.."/Source/",
		"%{prj.location}/"..targetName.."/include/",
		"%{prj.location}/"..targetName.."/NativeScripts/Generated"
	}

	if not _OPTIONS['server'] then
		includedirs
		{
			"%{IncludeDir.ImGui}",
		}
	end

	libdirs
	{
		"%{IncludeDir.Vulkan}/lib",
	}
	if _OPTIONS["hot-reload"] then
		libdirs
		{
			engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"
		}
	else
		libdirs
		{
			engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"
		}
	end


	links
	{
		"ImpulseEngine"
	}

	if _OPTIONS["build-editor"] then
	defines
	{
		"GE_EDITOR"
	}
	end

	if _OPTIONS["hot-reload"] then
	defines
	{
		"GE_DYNAMIC_LINK",
		"GE_PRJ_OFFSET=3"
	}
	else
	defines 
	{
		"GE_PRJ_OFFSET=3"
	}
	links
	{
		"NativeScripts"
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
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**" 
		}

		defines
		{
			"GE_PLATFORM_WINDOWS",
			"MS_BUILD_BIN=\"$(MSBuildBinPath)\""
		}
		
		if _OPTIONS["build-editor"] then
			postbuildcommands
			{
				"call \"%{wks.location}Tools/Packager.exe\" -zip \"%{prj.location}AndroidStudio\" \"%{prj.location}"..targetName.."/Content/Archives/AndroidStudio.zip\"",
				"call \"%{wks.location}Tools/Packager.exe\" -zip \"%{wks.location}vendor\" \"%{prj.location}"..targetName.."/Content/Archives/vendor.zip\"",
				"call \"%{wks.location}Tools/Packager.exe\" -zip \"%{prj.location}Generate\" \"%{prj.location}"..targetName.."/Content/Archives/Generate.zip\"",
				"call \"%{wks.location}Tools/Packager.exe\" -zip \"%{prj.location}BuildTarget.lua\" \"%{prj.location}"..targetName.."/Content/Archives/BuildTarget.zip\"",
				"call \"%{wks.location}Tools/Packager.exe\" -zip \"%{wks.location}Tools\" \"%{prj.location}"..targetName.."/Content/Archives/Tools.zip\"",
				"call \"%{wks.location}Tools/Packager.exe\" -zip \"%{prj.location}"..targetName.."/Source/Shared\" \"%{prj.location}"..targetName.."/Content/Archives/Shared.zip\"",
				"call \"%{wks.location}Tools/Packager.exe\" -pak \"%{prj.location}"..targetName.."/Engine/EngineContent\" \"%{prj.location}"..targetName.."/Data/EngineContent.pak\"",
				"call \"%{wks.location}Tools/Packager.exe\" -pak \"%{prj.location}"..targetName.."/Content\" \"%{prj.location}"..targetName.."/Data/EditorContent.pak\""
				
			}
		else 
			postbuildcommands
			{
				"call \"%{wks.location}Tools\\Packager.exe\" -pak \"$(ProjectDir)"..targetName.."/Content\" \"$(ProjectDir)"..targetName.."/Data/"..targetName.."Content.pak\""
			}
		end
		if _OPTIONS["hot-reload"] then
		postbuildcommands
		{
			"copy /Y  \""..engineSrc.."ImpulseEngine\\ImpulseEngine\\Bin\\".. outputdir.."\\ImpulseEngine\\shared\\ImpulseEngine.dll\" \"$(TARGETDIR)ImpulseEngine.dll\"",
			"copy /Y  \""..engineSrc.."ImpulseEngine\\ImpulseEngine\\Bin\\".. outputdir.."\\ImpulseEngine\\shared\\ImpulseEngine.pdb\" \"$(TARGETDIR)ImpulseEngine.pdb\""
		}
		end

		postbuildcommands
		{
			"xcopy /i /e /s /y \"$(ProjectDir)"..targetName.."/Data\" \"$(TargetDir)Data/\""
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
				linkoptions {"-F "..engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"}
			else
				linkoptions {"-F "..engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"}
			end
			


			links 
			{
				"Enet",
				"miniupnpc",
				"box2d",
				"dl",
				"pthread",
				"stdc++fs",
				"zlib",
				"zip"
			}
			if _OPTIONS['server'] then
			else
			links 
			{
				"ImGui",
				"freetype",
				"GL",
				"Glad",
				"GLFW",
				"X11",
				"openal",
				"Vorbis"
			}
			end

			excludes 
			{ 
				"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**" 
			}
	
			defines
			{
				"GE_PLATFORM_LINUX"
			}
	
			if _OPTIONS["build-editor"] then
				postbuildcommands
				{
					"\"%{wks.location}/Tools/Packager_Linux\" -zip \"%{prj.location}/AndroidStudio\" \"%{prj.location}/"..targetName.."/Content/Archives/AndroidStudio.zip\"",
					"\"%{wks.location}/Tools/Packager_Linux\" -zip \"%{wks.location}/vendor\" \"%{prj.location}/"..targetName.."/Content/Archives/vendor.zip\"",
					"\"%{wks.location}/Tools/Packager_Linux\" -zip \"%{prj.location}/Generate\" \"%{prj.location}/"..targetName.."/Content/Archives/Generate.zip\"",
					"\"%{wks.location}/Tools/Packager_Linux\" -zip \"%{prj.location}/BuildTarget.lua\" \"%{prj.location}/"..targetName.."/Content/Archives/BuildTarget.zip\"",
					"\"%{wks.location}/Tools/Packager_Linux\" -zip \"%{wks.location}/Tools\" \"%{prj.location}/"..targetName.."/Content/Archives/Tools.zip\"",
					"\"%{wks.location}/Tools/Packager_Linux\" -pak \"%{prj.location}/"..targetName.."/Engine/EngineContent\" \"%{prj.location}/"..targetName.."/Data/EngineContent.pak\"",
					"\"%{wks.location}/Tools/Packager_Linux\" -pak \"%{prj.location}/"..targetName.."/Content\" \"%{prj.location}/"..targetName.."/Data/EditorContent.pak\""
					
				}
			else 
				postbuildcommands
				{
					"\"%{wks.location}/Tools/Packager_Linux\" -pak \"%{prj.location}/"..targetName.."/Content\" \"%{prj.location}/"..targetName.."/Data/"..targetName.."Content.pak\""
				}
			end

			postbuildcommands
			{
				"cp -rf \"%{prj.location}/"..targetName.."/Data\" \"%{prj.location}/Bin/" .. outputdir .. "/"..targetName.."/Data\""
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
		linkoptions ("-F %{IncludeDir.firebase}/lib/ios" .. " -ObjC")
		if _OPTIONS["hot-reload"] then
			linkoptions {"-F "..engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"}
		else
			linkoptions {"-F "..engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"}
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
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.m",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.mm",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.c",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.h",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.storyboard",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**.plist",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/*.json",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/iOSImages.xcassets",
		}
			
		includedirs 
		{
			"%{prj.location}/"..targetName.."/Source",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS",
	
			"%{IncludeDir.firebase}/include"
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
			"cp -rf ${PROJECT_DIR}/"..targetName.."/Data ${TARGET_BUILD_DIR}/%{prj.name}.app/",
			"xattr -rc ."
			--"cp -rf ${PROJECT_DIR}/"..targetName.."/Source/iOS/GoogleService-Info.plist ${TARGET_BUILD_DIR}/%{prj.name}.app/"

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
			linkoptions {"-F "..engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared"}
		else
			linkoptions {"-F "..engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"}
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
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/Android/**.h",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/Android/**.c",
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/Android/**.cpp",
		}

		includedirs 
		{
			"%{prj.location}/"..targetName.."/Source",
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
				"%{IncludeDir.firebase}/lib/android/libx86"
			}
			androidLibDir = "x86"
			postbuildcommands
			{
			"mkdir \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib"..targetName..".so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/Bin/" .. outputdir .. "/"..targetName.."\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y /C \"$(ProjectDir)"..targetName.."/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}
		filter "platforms:x64"
			architecture "x64"
			libdirs
			{
				"%{IncludeDir.firebase}/lib/android/libx64"
			}
			androidLibDir = "x86_64"
			postbuildcommands
			{
			"mkdir \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"copy /y \"%{wks.location}x64\\%{cfg.buildcfg}\\lib"..targetName..".so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}x64\" \"%{prj.location}/Bin/" .. outputdir .. "/"..targetName.."\"",
			"RMDIR /Q/S \"%{wks.location}x64\"",
			"XCOPY /I /E /S /Y /C \"$(ProjectDir)"..targetName.."/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}
			
		filter "platforms:ARM"
			architecture "ARM"
			libdirs
			{
				"%{IncludeDir.firebase}/lib/android/libarm"
			}
			androidLibDir = "armeabi-v7a"
			postbuildcommands
			{
			"mkdir \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"copy /y \"%{wks.location}ARM\\%{cfg.buildcfg}\\lib"..targetName..".so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}ARM\" \"%{prj.location}/Bin/" .. outputdir .. "/"..targetName.."\"",
			"RMDIR /Q/S \"%{wks.location}ARM\"",
			"XCOPY /I /E /S /Y /C \"$(ProjectDir)"..targetName.."/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}

		filter "platforms:ARM64"
			architecture "ARM64"
			libdirs
			{
				"%{IncludeDir.firebase}/lib/android/libarm64"
			}
			androidLibDir = "arm64-v8a"
			postbuildcommands
			{
			"mkdir \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"copy /y \"%{wks.location}ARM64\\%{cfg.buildcfg}\\lib"..targetName..".so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}ARM64\" \"%{prj.location}/Bin/" .. outputdir .. "/"..targetName.."\"",
			"RMDIR /Q/S \"%{wks.location}ARM64\"",
			"XCOPY /I /E /S /Y /C \"$(ProjectDir)"..targetName.."/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}

	filter "system:macosx"
		systemversion "latest"
		kind "WindowedApp"
		buildoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}
		if _OPTIONS["hot-reload"] then
			linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib", "-F "..engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/shared", "-rpath @executable_path/"}
		else
			linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib", "-F "..engineSrc.."ImpulseEngine/ImpulseEngine/bin/".. outputdir .. "/ImpulseEngine/static"}
		end
		defines
		{
			"GE_PLATFORM_MACOSX"
		}

		excludes 
		{ 
			"%{prj.location}/"..targetName.."/Source/Shared/Engine/iOS/**" 
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


		if _OPTIONS["build-editor"] then
			postbuildcommands
			{
				"\"%{wks.location}/Tools/Packager_MacOSX\" -zip \"%{prj.location}/AndroidStudio\" \"%{prj.location}/"..targetName.."/Content/Archives/AndroidStudio.zip\"",
				"\"%{wks.location}/Tools/Packager_MacOSX\" -zip \"%{wks.location}/vendor\" \"%{prj.location}/"..targetName.."/Content/Archives/vendor.zip\"",
				"\"%{wks.location}/Tools/Packager_MacOSX\" -zip \"%{prj.location}/Generate\" \"%{prj.location}/"..targetName.."/Content/Archives/Generate.zip\"",
				"\"%{wks.location}/Tools/Packager_MacOSX\" -zip \"%{prj.location}/BuildTarget.lua\" \"%{prj.location}/"..targetName.."/Content/Archives/BuildTarget.zip\"",
				"\"%{wks.location}/Tools/Packager_MacOSX\" -zip \"%{wks.location}/Tools\" \"%{prj.location}/"..targetName.."/Content/Archives/Tools.zip\"",
				"\"%{wks.location}/Tools/Packager_MacOSX\" -zip \"%{prj.location}/"..targetName.."/Source/Shared\" \"%{prj.location}/"..targetName.."/Content/Archives/Shared.zip\"",
				"\"%{wks.location}/Tools/Packager_MacOSX\" -pak \"%{prj.location}/"..targetName.."/Content\" \"%{prj.location}/"..targetName.."/Data/EditorContent.pak\"",
				"\"%{wks.location}/Tools/Packager_MacOSX\" -pak \"%{prj.location}/"..targetName.."/Engine/EngineContent\" \"%{prj.location}/"..targetName.."/Data/EngineContent.pak\""
			}
		else 
			postbuildcommands
			{
				"\"%{wks.location}/Tools/Packager_MacOSX\" -pak \"%{prj.location}/"..targetName.."/Content\" \"%{prj.location}/"..targetName.."/Data/"..targetName.."Content.pak\""
			}
		end

		postbuildcommands
		{
			"cp -rf ${PROJECT_DIR}/"..targetName.."/\"Data\" ${TARGET_BUILD_DIR}/%{prj.name}.app/Contents/MacOS",
			--"cp -rf ${PROJECT_DIR}/"..targetName.."/\"Res\" ${TARGET_BUILD_DIR}",
		}

		if _OPTIONS["hot-reload"] then
			postbuildcommands
			{
				"cp -rf  \""..engineSrc.."ImpulseEngine/ImpulseEngine/Bin/".. outputdir.."/ImpulseEngine/shared/libImpulseEngine.dylib\" \"${TARGET_BUILD_DIR}/%{prj.name}.app/Contents/MacOS/libImpulseEngine.dylib\"",
			}
		end

		
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
				

