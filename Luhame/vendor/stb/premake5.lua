project "StbImage"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "stb_image.h",
        "stb_image.cpp"
    }

	filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        
