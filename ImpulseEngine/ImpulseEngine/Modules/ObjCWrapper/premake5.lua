project "ObjCWrapper"
    kind "StaticLib"
    language "C++"
    if _OPTIONS['hot-reload'] then
		staticruntime "off"
	else
		staticruntime "on"
	end
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/**.h",
    }

    includedirs
    {
        "include"
    }
    
    filter "system:macosx"
        systemversion "latest"
        xcodebuildsettings
        { 
            ["ALWAYS_SEARCH_USER_PATHS"] = "YES",
            ["SKIP_INSTALL"] = "YES"
        }
        
        files
        {
            "src/**.mm",
            "src/**.m"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

