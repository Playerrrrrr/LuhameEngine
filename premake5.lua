workspace "Luhame"
    architecture "x64"
    targetdir "build"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Luhame/vendor/GLFW/include"
IncludeDir["Glad"] = "luhame/vendor/glad/include"
IncludeDir["ImGui"] = "luhame/vendor/imgui"

include "Luhame/vendor/GLFW"--把该目录下的premake5.lua加载过来
include "Luhame/vendor/imgui"
include "Luhame/vendor/glad"

project "Luhame"
    location "Luhame"
    kind "SharedLib"
    language "C++"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Luhame/src/pch.cpp"

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
    }

    includedirs
	{
		"%{prj.name}/src",
        "%{prj.name}/vendor",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.Glad}"
    }

    links 
	{ 
		"GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }
    
	filter "system:windows"
		cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
		defines 
		{ 
            "LH_PLATFORM_WINDOWS",
            "LH_BUILD_DLL",
		}
			
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }		

    filter "configurations:Debug"
        defines "LH_DEBUG"
        symbols "On"
                
    filter "configurations:Release"
        defines "LH_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "LH_DIST"
        optimize "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    systemversion "latest"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
		"Luhame"
    }
    

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}
    
	includedirs 
	{
        "%{prj.name}/src",
        "Luhame/src",
        "Luhame/vendor",
    }
	
	filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        
		defines 
		{ 
            "LH_PLATFORM_WINDOWS",
		}
    
    filter "configurations:Debug"
        defines "LH_DEBUG"
        symbols "On"
                
    filter "configurations:Release"
        defines "LH_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "LH_DIST"
        optimize "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"     
