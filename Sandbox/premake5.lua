project "Sandbox"
    location "../Sandbox"
    kind "ConsoleApp"
    language "C++"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    links{
        "Luhame"
    }

	files 
	{ 
		"**.h", 
		"**.c", 
		"**.hpp", 
		"**.cpp" 
	}
    
    	includedirs 
	{
        "src",
        "../Luhame/src",
        "../Luhame/vendor",
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        
		links 
		{ 
			"Luhame"
		}
        
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