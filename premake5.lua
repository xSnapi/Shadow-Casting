-- premake5.lua
workspace "Shadows"
   startproject "Shadow Casting"
   architecture "x64"
   configurations {
      "Debug", 
      "Release" 
   }

   filter { "platforms:Win64" }
      system "Windows"

include "Shadow Casting"