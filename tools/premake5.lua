workspace "Wintergreen"
	location "../"
	architecture "x86"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

binoutputdir = "../%{prj.name}/build/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
intoutputdir = "../%{prj.name}/build/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Wintergreen"
	location "../Wintergreen"
	kind "StaticLib"
	language "C++"
	shadermodel "5.0"
	targetdir (binoutputdir)
	objdir (intoutputdir)

	files
	{
		"../%{prj.name}/src/**.h",
		"../%{prj.name}/src/**.cpp",
		"../%{prj.name}/src/shaders/**.hlsl"
	}

	includedirs
	{
		"../%{prj.name}/src"
	}

	filter "files:**-p.hlsl"
	shadertype "Pixel"

	filter "files:**-v.hlsl"
	shadertype "Vertex"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"WG_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			--{"{COPY} %{cfg.buildtarget.bundlepath} ../bin/" ..outputdir .. "/Sandbox"},
			--{"{COPY} %{cfg.buildtarget.bundlepath}/**.cso ../Sandbox"}
		}

	filter "configurations:Debug"
		defines "WG_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "WG_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "WG_DIST"
		optimize "On"

project "Sandbox"
	location "../Sandbox"
	kind "ConsoleApp"
	language "C++"
	targetdir (binoutputdir)
	objdir (intoutputdir)

	files
	{
		"../%{prj.name}/src/**.h",
		"../%{prj.name}/src/**.cpp",
		"../%{prj.name}/src/shaders/**.hlsl"
	}

	includedirs
	{
		"../Wintergreen/src"
	}

	links
	{
		"Wintergreen"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"WG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "WG_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "WG_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "WG_DIST"
		optimize "On"