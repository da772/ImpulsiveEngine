project "Packager"
    kind "ConsoleApp"
    language "C++"
        cppdialect "C++17"
    if _OPTIONS['hot-reload'] then
		staticruntime "off"
	else
		staticruntime "on"
	end
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "Source/**.cpp",
        "Source/**.h",
    }

    includedirs
    { 
        "../../Source/",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
		"%{IncludeDir.entt}",	
		"%{IncludeDir.cr}",
		"%{IncludeDir.vector}",
		"%{IncludeDir.reflection}",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "ImpulseEngine"
    }
    
    filter "system:macosx"
        systemversion "latest"
        xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
            ["SKIP_INSTALL"] = "YES"
        }

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
			"OpenAL.framework",
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
            "GE_PLATFORM_WINDOWS"
        }
        if _OPTIONS["hot-reload"] then
        postbuildcommands
        {
            "copy /Y  \"%{wks.location}ImpulseEngine\\ImpulseEngine\\Bin\\".. outputdir.."\\ImpulseEngine\\shared\\ImpulseEngine.dll\" \"$(TARGETDIR)ImpulseEngine.dll\"",
            "copy /Y  \"%{wks.location}ImpulseEngine\\ImpulseEngine\\Bin\\".. outputdir.."\\ImpulseEngine\\shared\\ImpulseEngine.pdb\" \"$(TARGETDIR)ImpulseEngine.pdb\""
        }
        end
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

        
    filter "system:linux"
        systemversion "latest"
        defines 
        {
            "GE_PLATFORM_LINUX"
        }
        if _OPTIONS['hot-reload'] then
        pic "On"
        end
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"