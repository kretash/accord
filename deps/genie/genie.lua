solution "accord"
	configurations{"debug", "release"}

	location "../../vs2017"
	targetdir "../../bin"
	language "C++"
	platforms "x64"

	includedirs {"../../include"}
	libdirs {"../../deps/glew", "../../deps/SDL"}
	links {"glew32s", "SDL2", "SDL2main", "SDL2test", "openGL32"}

	project "accord"
		kind "StaticLib"
		files { "../../include/**/**.cpp", "../../include/**/**.h", "../../include/**/**.hh" }
		files { "../../include/**.h", "../../src/accord/**.cc" }
		files { "../../src/glew/**.c", "../../src/glew/include/**.h" }

		configuration "debug"
			targetsuffix "-d" 
			defines { "_CRT_SECURE_NO_WARNINGS", "WIN32","_DEBUG", "DEBUG" }
			flags { "Symbols" }

		configuration "release"
			defines { "WIN32", "NDEBUG" }
			flags { "Optimize" }

	project "example"
		links "accord"
		kind "ConsoleApp"
		files { "../../include/accord/**.h", "../../include/example/**.h" }
		files {"../../src/example/**.cc"}
		
		configuration "debug"
			targetsuffix "-d" 
			defines { "_CRT_SECURE_NO_WARNINGS", "WIN32","_DEBUG", "DEBUG" }
			flags { "Symbols" }

		configuration "release"
			defines { "WIN32", "NDEBUG" }
			flags { "Optimize" }