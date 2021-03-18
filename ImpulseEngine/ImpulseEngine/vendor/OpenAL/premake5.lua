project "OpenAL"
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
        "src/",
        "src/al/",
        "src/alc/",
        "src/common/",
        "include/"
    }

    files
    {
        "src/al/**.h",
        "src/al/**.cpp",
        "src/alc/*.h",
        "src/alc/*.cpp",
        "src/alc/*.h",
        "src/alc/effects/*.cpp",
        "src/alc/effects/*.h",
        "src/alc/filters/*.cpp",
        "src/alc/filters/*.h",
        "src/alc/mixer/mixer_c.cpp",
        "src/alc/mixer/*.h",
        "src/alc/backends/base.cpp",
        "src/alc/backends/base.h",
        "src/alc/backends/loopback.cpp",
        "src/alc/backends/loopback.h",
        "src/alc/backends/null.cpp",
        "src/alc/backends/null.h",
        "src/alc/backends/wave.cpp",
        "src/alc/backends/wave.h",
        "src/*.h",
        "include/**.h",
        "src/common/*.h",
        "src/common/*.cpp"

    }


    defines 
    {
        "OpenAL_EXPORTS",
        "AL_BUILD_LIBRARY",
        "AL_ALEXT_PROTOTYPES",
        "NOMINMAX",
        "AL_LIBTYPE_STATIC",
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    filter "system:macosx"
        systemversion "latest"

        defines 
        {
            ""
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

        defines 
        {
            "_ANDROID"
        }
        
        files
        {
            "src/alc/backends/opensl.h",
            "src/alc/backends/opensl.cpp"
        }

        filter "platforms:x86"
            architecture "x86"
            defines
            {
                "x86"
            }
            files 
            {
                "src/alc/mixer/mixer_sse.cpp",
                "src/alc/mixer/mixer_sse2.cpp",
                "src/alc/mixer/mixer_sse3.cpp",
                "src/alc/mixer/mixer_sse41.cpp",
            }
        filter "platforms:ARM"
            architecture "ARM"
            defines
            {
                "ARM"
            }
            files 
            {
                "src/alc/mixer/mixer_neon.cpp"
            }
        filter "platforms:ARM64"
            architecture "ARM64"
            defines
            {
                "ARM"
            }
            files 
            {
                "src/alc/mixer/mixer_neon.cpp"
            }
        filter "platforms:x64"
            architecture "x64"
            defines
            {
                "x86"
            }
            files 
            {
                "src/alc/mixer/mixer_sse.cpp",
                "src/alc/mixer/mixer_sse2.cpp",
                "src/alc/mixer/mixer_sse3.cpp",
                "src/alc/mixer/mixer_sse41.cpp",
            }

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

        disablewarnings 
        {
            "5030", "4834", "4069", "4996", "4065"
        }
        
        files
        {
            "src/alc/backends/dsound.cpp",
            "src/alc/backends/dsound.h",
            "src/alc/backends/winmm.h",
            "src/alc/backends/winmm.cpp",
            "src/alc/backends/wasapi.h",
            "src/alc/backends/wasapi.cpp",
            "src/alc/mixer/mixer_sse.cpp",
            "src/alc/mixer/mixer_sse2.cpp",
            "src/alc/mixer/mixer_sse3.cpp",
            "src/alc/mixer/mixer_sse41.cpp",
        }

        defines 
        {
            "_WINDOWS",
            "x86"
        }

        
        links
        {
            "dsound",
            "dxguid",
            "winmm"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
        
        filter "action:gmake2"
			cppdialect "gnu++17"
			defines
			{
                "GE_MINGW_",
			}
        
