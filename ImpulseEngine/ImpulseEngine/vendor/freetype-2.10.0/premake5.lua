project "freetype"
    kind "StaticLib"
    architecture "x64"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/cff/cff.c",
        "src/cid/type1cid.c",
        "src/psaux/psaux.c",
        "src/psnames/psnames.c",
        "src/smooth/smooth.c",
        "src/sfnt/sfnt.c",
        "src/truetype/truetype.c",
        "src/type1/type1.c",
        "src/raster/raster.c",
        "src/otvalid/otvalid.c",
        "src/pshinter/pshinter.c",
        "src/gzip/ftgzip.c",
        "src/pfr/**.c",
        "src/pcf/**.c",
        "src/bdf/**.c",
        "src/type42/**.c",
        "src/winfonts/**.c",
        "src/autofit/autofit.c",
        "src/lzw/**.c",

        "src/cff/cff.h",
        "src/cid/type1cid.h",
        "src/psaux/psaux.h",
        "src/psnames/psnames.h",
        "src/smooth/smooth.h",
        "src/sfnt/sfnt.h",
        "src/truetype/truetype.h",
        "src/type1/type1.h",
        "src/raster/raster.h",
        "src/otvalid/otvalid.h",
        "src/pshinter/pshinter.h",
        "src/gzip/ftgzip.h",
        "src/pfr/**.h",
        "src/pcf/**.h",
        "src/bdf/**.h",
        "src/type42/**.h",
        "src/winfonts/**.h",
        "src/lzw/**.h",


        "src/base/ftbase.h",
        "src/base/ftbbox.h",
        "src/base/ftbitmap.h",
        "src/base/ftgasp.h",
        "src/base/ftglyph.h",
        "src/base/ftinit.h",
        "src/base/ftstroke.h",
        "src/base/ftsynth.h",
        "src/base/ftsystem.h",
        "src/base/fttype1.h",
        "src/base/ftotval.h",
        "src/base/ftdebug.h",

        "src/base/ftbase.c",
        "src/base/ftbbox.c",
        "src/base/ftbitmap.c",
        "src/base/ftgasp.c",
        "src/base/ftglyph.c",
        "src/base/ftinit.c",
        "src/base/ftstroke.c",
        "src/base/ftsynth.c",
        "src/base/ftsystem.c",
        "src/base/fttype1.c",
        "src/base/ftotval.c",
        "src/base/ftdebug.c",


        "src/freetype-gl/**.c",
        "src/freetype-gl/**.h",


    }

    includedirs
    {
        "include/",
        "include/freetype",
        "include/freetype/config",
        "include/freetype/internal",
        "include/freetype/freetype-gl",
        "../../../%{IncludeDir.Glad}"
    }

    disablewarnings 
    {
        "4018",
        "4996",
        "4006"
    }

    defines 
    {
        "FT2_BUILD_LIBRARY",
        "FREETYPE_GL_ES_VERSION_3_0"
    }
    
    configuration "debug"
        defines 
        {
            "FT_DEBUG_LEVEL_ERROR",
            "FT_DEBUG_LEVEL_TRACE",
            "FT_DEBUG_AUTOFIT"
        }
        files
        {
            
        }

    configuration "release"
        defines
        {
            "NDEBUG"
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
        linkoptions { "-lm" }
        rtti ("On")
        cppdialect "gnu++17"
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
        filter "action:gmake2"
            cppdialect "gnu++17"

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
            "GL_WITH_GLAD"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
