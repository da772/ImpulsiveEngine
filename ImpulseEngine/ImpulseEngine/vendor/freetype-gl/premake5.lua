project "freetype-gl"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
    if _OPTIONS['hot-reload'] then
		staticruntime "off"
	else
		staticruntime "on"
	end
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    includedirs 
    {
        "../../../%{IncludeDir.freetype}",
        "../../../%{IncludeDir.Glad}"
    }
    
    files
    {
        "src/**.h",
        "src/**.c"
    }
    links
    {
        "freetype"
    }

    defines 
    {
        "FREETYPE_GL_ES_VERSION_3_0"
    }
    
    filter "system:macosx"
        systemversion "latest"

        defines 
        {
            "GL_WITH_GLAD"
        }
        
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

        defines 
        {
            "GL_WITH_GLAD"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"


    
    filter "system:linux"
        systemversion "latest"
        cppdialect "gnu++17"
        defines 
        {
            "GL_WITH_GLAD"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

        
