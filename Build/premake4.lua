solution "libRocketCircularBar"
    configurations { "Debug", "Release" } 

    project "libRocketCircularBar"
    
        rocket_dir = os.getenv( "LIBROCKET" ) .. "/"
        
        defines { "STATIC_LIB" }
        kind "StaticLib"
        language "C++"
        files { "../Source/CircularBar/**.cpp", "../Source/CircularBar/**.h" }
        includedirs{ rocket_dir .. "Include/", "../Include/" }
        targetdir "../Bin/"
        
        configuration "Debug"
            targetname "RocketCircularBar_d"
            libdirs{ rocket_dir .. "Build/Debug" }
            defines { "DEBUG" }
            flags { "Symbols" }
            links { "RocketCore_d" }
            
        configuration "Release"
            targetname "RocketCircularBar"
            libdirs{ rocket_dir .. "Build/Release" }
            defines { "NDEBUG" }
            links { "RocketCore" }
            
        configuration {}