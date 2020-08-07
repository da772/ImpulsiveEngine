project "miniupnpc"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/*.h",
        "src/*.c",
        "src/*.cpp"
    }
    
    filter "system:macosx"
        systemversion "latest"
        xcodebuildsettings
        { 
            ["ALWAYS_SEARCH_USER_PATHS"] = "YES",
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
        linkoptions { "-lm" }
        rtti ("On")
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

    filter "system:windows"
        systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
