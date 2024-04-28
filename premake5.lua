workspace "RealEngine"

	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "RealEngine"
	location "RealEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**h",
		"%{prj.name}/src/**cpp",
	}
		
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include;"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RE_PLATFORM_WINDOWS",
			"RE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "RE_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "RE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RE_Dist"
		optimize "On"



	workspace "RealEngine"

	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sandbox"
	location "RealEngine"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**h",
		"%{prj.name}/src/**cpp"
	}

	includedirs
	{
		"RealEngine/vendor/spdlog/include;",
		"RealEngine/src"
	}

	links{
		"RealEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RE_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "RE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RE_Dist"
		optimize "On"