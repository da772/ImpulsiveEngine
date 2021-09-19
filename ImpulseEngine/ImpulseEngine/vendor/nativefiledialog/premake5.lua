project "NativeFileDialog"
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
        "src/**.h",
    }

    includedirs
    {
        "src/include"
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
            "src/nfd_cocoa.m"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:windows"
        systemversion "latest"
        files
        {
            "src/nfd_win.cpp"
        }
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:linux"
        systemversion "latest"
        files
        {
            "src/nfd_gtk.cpp"
        }
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
