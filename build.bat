@echo off

set code=%cd%
set shader_dir=%code%\shaders
set build_dir=%code%\build
set lib_dir=%build_dir%\lib\release
set win32_dir=%build_dir%\win32
set glfw_dir=%build_dir%\glfw

REM external code include paths
set external=%code%\external
set glew=%external%\glew-2.1.0
set glfw=%external%\glfw-3.3.8\include
set imgui=%external%\imgui-1.89.5
set glm=%external%\glm-0.9.9.8
set freetype=%external%\freetype-2.13.0
set stb=%external%\stb
set assimp=%external%\assimp-5.2.5
set sqlite=%external%\sqlite

set libs=User32.lib Opengl32.lib Gdi32.lib  glew32.lib glfw3dll.exp glfw3dll.lib imgui*.obj freetype.lib assimp-vc143-mt.lib
set includes=/I%glew% /I%shader_dir% /I%glfw% /I%glm% /I%imgui% /I%freetype% /I%stb% /I%assimp% /I%sqlite%

set opts=-DDEBUG -D_CRT_SECURE_NO_WARNINGS -FC -GR- -EHa- -nologo -Od -Zo -Oi -fp:fast -fp:except- -Gm- -GR- -EHa- -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 %includes%

REM pushd dll
REM cl /LDd %debug_opts% %code%\app.cpp /link /LIBPATH:%lib_dir% %libs%
REM echo Copying dll build files to platform directories...
REM xcopy /s/y/v/q %cd%\app.* %glfw_dir%
REM xcopy /s/y/v/q %cd%\app.* %win32_dir%
REM popd

pushd build
pushd glfw
cl %opts% %code%\glfw_win32_main.cpp -Feapp_glfw /link /LIBPATH:%lib_dir% %libs% 
popd
popd



