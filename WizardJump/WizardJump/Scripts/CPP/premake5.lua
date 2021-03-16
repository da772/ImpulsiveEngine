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
    end
	
    filter "configurations:Debug"
        defines "GE_DEBUG"
        runtime "Debug"
        symbols "On"
    filter "configurations:Release"
        defines "GE_RELEASE"
        runtime "Release"
        optimize "On"
    filter "configurations:Dist"
        defines "GE_DIST"
        runtime "Release"
        optimize "On"