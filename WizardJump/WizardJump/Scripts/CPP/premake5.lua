project "Scripts_CPP"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    
    if _OPTIONS["hot-reload"] then
        staticruntime "off"
        kind "SharedLib"
    else 
        staticruntime "on"
    end
    targetdir ("%{wks.location}/WizardJump/Bin/" .. outputdir .. "/WizardJump")
	objdir ("%{wks.location}/WizardJump/Bin-Obj/" .. outputdir .. "/WizardJump")

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
        "%{wks.location}/ImpulseEngine/ImpulseEngine/vendor/spdlog/include",
		"%{wks.location}/ImpulseEngine/ImpulseEngine/Source",
		"%{wks.location}/ImpulseEngine/ImpulseEngine/vendor",
		"%{wks.location}/ImpulseEngine/%{IncludeDir.glm}",
		"%{wks.location}/ImpulseEngine/%{IncludeDir.entt}",	
		"%{wks.location}/ImpulseEngine/%{IncludeDir.cr}",
		"%{wks.location}/ImpulseEngine/%{IncludeDir.vector}",
        "%{wks.location}/ImpulseEngine/%{IncludeDir.reflection}",
    }
    if _OPTIONS["hot-reload"] then
    links 
    {
        "ImpulseEngine"
    }
    defines 
    {
        "GE_LOADED_DLL"
    }
    end

    filter "system:linux"
        if _OPTIONS['hot-reload'] then
        pic "On"
        end

    filter "system:macosx"
        defines
        {
            "GE_PLATFORM_MACOSX"
        }

        excludes 
        { 
            "%{prj.location}/%{prj.name}/Source/Engine/iOS/**" 
        }

        includedirs
        {

        }
        
        xcodebuildsettings
        { 
            ["ALWAYS_SEARCH_USER_PATHS"] = "YES"
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