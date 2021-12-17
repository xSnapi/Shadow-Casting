project "Shadow Casting"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "on"
  
   targetdir "bin/%{cfg.buildcfg}"
   objdir("bin/Obj/%{cfg.buildcfg}")

   files {
      "**.h", 
      "**.hpp",
	  "**.cpp",
   }

   includedirs {
      "../SFML/include",
   }

   defines { 
      "SFML_STATIC" 
   }

   linkoptions { 
      "/ignore:4006" 
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      libdirs { 
         "../SFML/lib/Debug",
         "../SFML/extlibs",
      }

      links {
         "opengl32.lib",
         "openal32.lib",
         "freetype.lib",
         "winmm.lib",
         "gdi32.lib",
         "flac.lib",
         "vorbisenc.lib",
         "vorbisfile.lib",
         "vorbis.lib",
         "ogg.lib",
         "ws2_32.lib",
         "legacy_stdio_definitions.lib",
         "sfml-graphics-s-d.lib",
         "sfml-window-s-d.lib",
         "sfml-system-s-d.lib",
         "sfml-audio-s-d.lib",
         "sfml-network-s-d.lib"
      }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      libdirs { 
         "../SFML/lib/Release",
         "../SFML/extlibs",
      }

      links {
         "opengl32.lib",
         "openal32.lib",
         "freetype.lib",
         "winmm.lib",
         "gdi32.lib",
         "flac.lib",
         "vorbisenc.lib",
         "vorbisfile.lib",
         "vorbis.lib",
         "ogg.lib",
         "ws2_32.lib",
         "legacy_stdio_definitions.lib",
         "sfml-graphics-s.lib",
         "sfml-window-s.lib",
         "sfml-system-s.lib",
         "sfml-audio-s.lib",
         "sfml-network-s.lib"
      }

