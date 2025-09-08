project "Luhame"
    location "../Luhame"
    kind "SharedLib"
    language "C++"

    targetdir("../bin/" .. outputdir .. "/%{prj.name}")
    objdir("../bin-int/" .. outputdir .. "/%{prj.name}")

    --定义预编译头
    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files{--项目中哪些文件用来编译
		"src/**.h", 
		"src/**.c", 
		"src/**.hpp", 
		"src/**.cpp" 
    }

    includedirs--依赖文件
	{
		"src",
		"vendor",
        "../%{IncludeDir.GLFW}"
	}

    links{
        "GLFW"
    }

    filter "system:windows"
		cppdialect "C++17"
        staticruntime "On"
        
		defines --在windows环境下定义宏
		{ 
            "LH_PLATFORM_WINDOWS",
            "LH_BUILD_DLL",
		}
        
        postbuildcommands
        {
            --将%{cfg.buildtarget.relpath}（Luhame生成的dll路径） 复制到../bin/" .. outputdir .. "/Sandbox" (sandbox.exe下)
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
