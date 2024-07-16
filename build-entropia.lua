workspace "Daedalus"
	configurations { "run", "static-build", "dynamic-build" }
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

	filter { "configurations:run" }
		kind "ConsoleApp"

	filter { "configurations:static-build" }
		kind "StaticLib"
		removefiles { "main.cpp" }

	filter { "configurations:dynamic-build" }
		kind "SharedLib"
		removefiles { "main.cpp" }