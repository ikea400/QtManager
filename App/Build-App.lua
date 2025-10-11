include( "../../../premake-qt/qt.lua" )
local qt = premake.extensions.qt

project "App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   qt.enable()
   qtuseexternalinclude ( true )
   qtmodules { "core", "gui", "widgets", "network", "svg" }
   qtpath "E:/Qt/6.8.0/msvc2022_64"
   qtprefix "Qt6"

   files { "Source/**.h", "Source/**.cpp", "Source/**.ts", "Source/**.ui", "Source/**.qrc" }

   includedirs
   {
      "Source",

	  -- Include Common
	  "../Common/Source"
   }

   links
   {
      "Common"
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
       qtsuffix "d"

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