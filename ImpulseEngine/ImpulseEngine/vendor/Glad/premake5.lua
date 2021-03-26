project "Glad"
    kind "StaticLib"
    language "C"
    if _OPTIONS['hot-reload'] then
		staticruntime "off"
	else
		staticruntime "on"
	end
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

	includedirs
	{
		"include"
	}
    
    xcodebuildsettings
    { 
        ["ALWAYS_SEARCH_USER_PATHS"] = "YES" 
    }


    filter "system:android"
        architecture "ARM"
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

    filter "system:windows"
        systemversion "latest"
        
	
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:linux"
        systemversion "latest"
        if _OPTIONS['hot-reload'] then
            pic "On"
        end
    
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
        
