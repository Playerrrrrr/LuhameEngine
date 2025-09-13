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
IncludeDir["Glad"] = "Luhame/vendor/glad/include"
IncludeDir["ImGui"] = "Luhame/vendor/imgui"
IncludeDir["glm"] = "Luhame/vendor/glm"
IncludeDir["stb"] = "Luhame/vendor/stb"
IncludeDir["spdlog"] = "Luhame/vendor/spdlog/include"
IncludeDir["assimp"] = "Luhame/vendor/assimp/include"

include "Luhame/vendor/GLFW"--把该目录下的premake5.lua加载过来
include "Luhame/vendor/imgui"
include "Luhame/vendor/glad"
include "Luhame/vendor/stb"

project "Luhame"
    location "Luhame"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    
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
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.assimp}"
    }

    links 
	{ 
		"GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "StbImage"
    }

    defines { "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE" }

	filter "system:windows"
		cppdialect "C++17"
        
		defines 
		{ 
            "LH_PLATFORM_WINDOWS",
            "LH_BUILD_LIB",
		}
			
        -- dll do
       -- postbuildcommands
       -- {
       --     ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
       -- }		

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
        buildoptions "/MT /execution-charset:utf-8" --for spdlog
    filter { "system:windows", "configurations:Debug" }
        buildoptions "/execution-charset:utf-8" --for spdlog
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"
    cppdialect "C++17"
	
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
		"Luhame",
        "Luhame/vendor/assimp/win64/assimp.lib"
    }
    
   defines { "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE" }

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" ,

	}
    
	includedirs 
	{
        "%{prj.name}/src",
        "Luhame/src",
        "Luhame/vendor",
        "%{IncludeDir.glm}",
    }
	
	filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
		defines 
		{ 
            "LH_PLATFORM_WINDOWS",
		}
    
    filter "configurations:Debug"
        defines "LH_DEBUG"
        symbols "on"
                
    filter "configurations:Release"
        defines "LH_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "LH_DIST"
        optimize "on"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT /execution-charset:utf-8" --for spdlog
    filter { "system:windows", "configurations:Debug" }
        buildoptions "/execution-charset:utf-8" --for spdlog
