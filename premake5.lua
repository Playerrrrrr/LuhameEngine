workspace "Luhame"
    architecture "x64"
    targetdir "build"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Luhame/vendor/GLFW/include"

include "Luhame/vendor/GLFW"--把该目录下的premake5.lua加载过来
include "Luhame"
include "Sandbox"
