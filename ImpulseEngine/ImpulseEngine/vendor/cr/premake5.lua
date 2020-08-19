project "crMain"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
    staticruntime "On"
    
    targetdir (path.getabsolute(appLocation))
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "source/**"
    }

    includedirs
    {
        "include/"
    }

    defines 
    {
        "DLL_LOCATION=\""..appName .. "\""
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
        exceptionhandling ("On")
        rtti ("On")
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

        
		postbuildcommands
		{
			--"copy /y  \"$(TargetPath)\" \"" .. path.getabsolute(appLocation) ..  "/%{prj.name}.exe\""
		}

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

        
