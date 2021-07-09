project "NativeScripts"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    
    if _OPTIONS["hot-reload"] then
        staticruntime "off"
        kind "SharedLib"
    else 
        staticruntime "on"
    end
    targetdir ("%{wks.location}/".. targetName .."/Bin/" .. outputdir .. "/".. targetName .."")
	objdir ("%{wks.location}/".. targetName .."/Bin-Obj/" .. outputdir .. "/".. targetName .."")

    files 
    {
        "%{prj.location}/Scripts/**.cpp",
        "%{prj.location}/Scripts/**.c",
        "%{prj.location}/Scripts/**.h",
        "%{prj.location}/Scripts/**.hpp",
        "%{prj.location}/Generated/**.hpp",
        "%{prj.location}/Generated/**.h",
        "%{prj.location}/Generated/**.cpp"
    }

    includedirs
    {
		engineSrc.."/ImpulseEngine/ImpulseEngine/Source",
		engineSrc.."/ImpulseEngine/ImpulseEngine/vendor",
        "%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.vector}",
        "%{IncludeDir.reflection}",
        "%{IncludeDir.ImGui}",
        "Scripts"
    }
    if _OPTIONS["hot-reload"] then
    links 
    {
        "ImpulseEngine"
    }
    defines 
    {
        "GE_LOADED_DLL",
        "GE_DYNAMIC_LINK"
    }
    end
    if _OPTIONS["server"] then
        defines 
        {
            "GE_SERVER_APP"
        }
    end

    filter "system:windows"
		systemversion "latest"
        defines 
        {
            "GE_PLATFORM_WINDOWS"
        }
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:linux"
        if _OPTIONS['hot-reload'] then
        pic "On"
        end
        defines 
        {
            "GE_PLATFORM_LINUX"
        }
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:macosx"
        defines
        {
            "GE_PLATFORM_MACOSX"
        }
        
        links
        {
            "Cocoa.framework",
			"OpenGL.framework",
			"IOKit.framework",
			"CoreVideo.framework",
			"OpenAL.framework"
        }
        
		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES" ,
			["SKIP_INSTALL"] = "YES"
		}

    filter "system:ios"
		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
            ["ARCHS"] = "$(ARCHS_STANDARD)",
            ["SKIP_INSTALL"] = "YES"
        }
        
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:android"
        cppdialect "gnu++17"
        rtti ("On")
        linkoptions { "-lm" }
        exceptionhandling ("On")
        androidapilevel(android_version)
        
        filter "platforms:x86"
            architecture "x86"
        filter "platforms:ARM"
            architecture "ARM"
        filter "platforms:ARM64"
            architecture "ARM64"
        filter "platforms:x64"
			architecture "x64"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
        filter "configurations:Dist"
            defines "GE_DIST"
            runtime "Release"
            optimize "On"