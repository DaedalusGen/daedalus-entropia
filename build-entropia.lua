workspace "Daedalus"
	configurations { "DEBUG", "RELEASE" }
	platforms { "run", "static-build", "dynamic-build" }
	startproject "Daedalus-Entropia"

include "daedalus-core/src/include-core.lua"

project "Daedalus-Entropia"
	language "C++"
	location "build/daedalus-entropia"
	entrypoint "main"

	files {
		"src/main.cpp",
		"src/daedalus-entropia/**/*.cpp",
		"src/include/**/*.hpp",
	}

	includedirs {
		"src/include",
		"daedalus-core/src/include/"
	}

	links { "Daedalus-Core" }

	filter { "platforms:run" }
		kind "ConsoleApp"

	filter { "platforms:static-build" }
		kind "StaticLib"
		removefiles { "main.cpp" }

	filter { "platforms:dynamic-build" }
		kind "SharedLib"
		removefiles { "main.cpp" }

	filter { "configurations:debug" }
	    defines { "DEBUG" }
