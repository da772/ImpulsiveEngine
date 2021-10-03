
newoption {
	trigger = "build-openal",
	description = "builds open al"
}

newoption {
	trigger = "server",
	description = "build as standalone server"
}

newoption {
	trigger = "disable-nfd",
	description = "disable native file dialog"
}


if _OPTIONS['build-engine'] then

group "Dependencies"
	if not _OPTIONS['server'] then
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/GLFW")
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/Glad")
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/imgui")
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/freetype-2.10.0")
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/Vorbis")
	end
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/Enet")
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/miniupnpc")
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/zlib")
	
	--include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/freetype-gl"
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/box2d")
	if _OPTIONS['build-openal'] then
		include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/OpenAL")
	end

	if _OPTIONS['os'] == "macosx" then
		include(vendorSrc.."ImpulseEngine/ImpulseEngine/Modules/ObjCWrapper")
	end

	if (_OPTIONS['os'] == "macosx" or _OPTIONS['os'] == "windows" or _OPTIONS['os'] == "linux") then
		if (not _OPTIONS['disable-nfd'] and not _OPTIONS['server']) then
			include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/nativefiledialog")
		end
		include(vendorSrc.."ImpulseEngine/ImpulseEngine/vendor/zip")
	end
	
if not _OPTIONS['hot-reload'] and not _OPTIONS['server'] then
group "Tools"
	include(vendorSrc.."ImpulseEngine/ImpulseEngine/Tools/Packager")
end
group ""

project "ImpulseEngine"
	location "ImpulseEngine"
	language "C++"
	cppdialect "C++17"
	if _OPTIONS['hot-reload'] then
	staticruntime "off"
	kind "SharedLib"
	defines
	{
		"GE_HOT_RELOAD",
		"GE_DYNAMIC_LINK",
		"GE_BUILD_DLL"
	}
	targetdir ("%{prj.name}/Bin/" .. outputdir .. "/%{prj.name}/shared")
	objdir ("%{prj.name}/Bin-Obj/" .. outputdir .. "/%{prj.name}/shared")
	else
	staticruntime "on"
	kind "StaticLib"
	targetdir ("%{prj.name}/Bin/" .. outputdir .. "/%{prj.name}/static")
	objdir ("%{prj.name}/Bin-Obj/" .. outputdir .. "/%{prj.name}/static")
	end
	if _OPTIONS['server'] then
	defines 
	{
		"GE_SERVER_APP"	
	}
	end

	if _OPTIONS['disable-nfd'] then
		defines
		{
			"GE_DISABLE_NFD"
		}
	end	

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
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.ENET}",
		"%{IncludeDir.httplib}",
		"%{IncludeDir.miniupnpc}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.firebase}/include",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.vector}",
		"%{IncludeDir.reflection}",
	}

	if not _OPTIONS['server'] then
		includedirs
		{
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.freetypegl}",
			"%{IncludeDir.OpenAL}",
			"%{IncludeDir.Vorbis}",
			"%{IncludeDir.gltext}",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.Vulkan}/include",
		}
	end

	libdirs 
	{
		"%{IncludeDir.Vulkan}/lib"
	}
	if _OPTIONS['server'] then
	links
	{
		"Enet",
		"miniupnpc",
		"zlib",
		"box2d"
		
	}
	else
	links
	{
		"ImGui",
		"freetype",
		"Vorbis",
		"Enet",
		"miniupnpc",
		"zlib",
		"box2d"
	}
	end
	
	
	if _OPTIONS['build-openal'] then	
	links
	{
		"OpenAL"	
	}
	end

	filter "system:macosx"
		systemversion "latest"
		
		buildoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}
		linkoptions {"-F /System/Library/Frameworks", "-F %{IncludeDir.Vulkan}/lib"}

		files
		{
			"%{prj.name}/Source/**.m"
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
			"%{IncludeDir.nfd}",
			"%{IncludeDir.objc}",
			"%{IncludeDir.zip}"
		}

		links 
		{
			"Cocoa.framework",
			"OpenGL.framework",
			"IOKit.framework",
			"CoreVideo.framework",
			--"libvulkan.1.1.130",
			"OpenAL.framework",
			"Glad",
			"GLFW",
			"ObjCWrapper",
			"zip"
		}

		if (not _OPTIONS['disable-nfd'] and not _OPTIONS['server']) then
		links
		{
			"NativeFileDialog"
		}
		end

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

		filter "action:xcode4"
			pchheader "Source/gepch.h"
			pchsource "Source/gepch.cpp"
			
		filter "action:gmake2"
			pchheader "gepch.h"
			pchsource "ImpulseEngine/Source/gepch.cpp"
		
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
			"%{IncludeDir.nfd}",
			"%{IncludeDir.zip}"
		}

		defines
		{
			"GE_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"GE_WINDOW_GLFW",
			"GE_PLATFORM_WINDOWS",
			"GL_WITH_GLAD",
			"MS_BUILD_BIN=\"$(MSBuildBinPath)\""
		}

		if _OPTIONS['server'] then
		links 
		{
			"Ws2_32.lib",
			"shlwapi.lib"
		}
		else
		links 
		{
			"opengl32.lib",
			"vulkan-1.lib",
			"Glad",
			"GLFW",
			"Ws2_32.lib",
			"shlwapi.lib",
			"zip"
		}
		if (not _OPTIONS['disable-nfd']) then
			links
			{
				"NativeFileDialog"
			}
		end
		end

	

		

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

	filter "system:linux"
		linkgroups 'on'
		systemversion "latest"
		pchheader "gepch.h"
		pchsource "ImpulseEngine/Source/gepch.cpp"
		cppdialect "gnu++17"
		if _OPTIONS['hot-reload'] then
			pic "On"
		end
		files
		{
		}
		
		
		includedirs
		{
			"%{IncludeDir.nfd}",
			"%{IncludeDir.zip}",
		}

		defines
		{
			"GLFW_INCLUDE_NONE",
			"GE_WINDOW_GLFW",
			"GE_PLATFORM_LINUX",
			"GL_WITH_GLAD"
		}
		
		if not _OPTIONS['server'] then
		links 
		{
			"GL",
			"Glad",
			"GLFW",
			"X11",
			"zip"
		}
		if (not _OPTIONS['disable-nfd']) then
			links
			{
				"NativeFileDialog"
			}
		end

		end

	

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

end