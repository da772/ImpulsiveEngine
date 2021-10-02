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
    }

    if not _OPTIONS['server'] then
        "%{IncludeDir.ImGui}"
    end

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

        targetname "Packager_MacOSX"

        postbuildcommands
        {
            "cp -f ./Bin/" .. outputdir .. "/%{prj.name}/Packager_MacOSX %{wks.location}/Tools/",
            "chmod +x %{wks.location}/Tools/Packager_MacOSX"
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
            "copy /Y  \"%{wks.location}ImpulseEngine\\ImpulseEngine\\Bin\\".. outputdir.."\\ImpulseEngine\\shared\\ImpulseEngine.pdb\" \"$(TARGETDIR)ImpulseEngine.pdb\"",
        }
        end

        postbuildcommands 
        {
            "copy /Y  %{prj.location}Bin\\" .. outputdir .. "\\%{prj.name}\\Packager.exe %{wks.location}Tools\\Packager.exe"
        }
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

        targetname "Packager_Linux"

        postbuildcommands
        {
            "cp -f ./Bin/" .. outputdir .. "/%{prj.name}/Packager_Linux %{wks.location}/Tools/",
            "chmod +x %{wks.location}/Tools/Packager_Linux"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
        