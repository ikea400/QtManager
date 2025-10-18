project "Common"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
       "Source",
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WINDOWS" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "RELEASE", "NDEBUG" }
      runtime "Release"
      optimize "On"
      symbols "On"
      flags { "linktimeoptimization" }

   filter "configurations:Dist"
      defines { "DIST", "NDEBUG" }
      runtime "Release"
      optimize "On"
      symbols "Off"
      flags { "linktimeoptimization" }
    
   filter "configurations:Test"
       defines { "TEST", "NDEBUG" }
       runtime "Release"
       optimize "On"
       symbols "Off"