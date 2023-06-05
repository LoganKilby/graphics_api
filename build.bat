@echo off

set code=%cd%
set shader_dir=%code%\shaders
set build_dir=%code%\build
set lib_dir=%build_dir%\lib\release
set win32_dir=%build_dir%\win32
set glfw_dir=%build_dir%\glfw

set external_include_dir=%code%\external
set glew_include_dir=%code%\external\glew-2.1.0
set glfw_include_dir=%code%\external\glfw-3.3.8\include
set imgui_include_dir=%code%\external\imgui-1.89.5
set glm_include_dir=%code%\external\glm-0.9.9.8
set freetype_include_dir=%code%\external\freetype-2.13.0\include

set libs=User32.lib Opengl32.lib Gdi32.lib  glew32.lib glfw3dll.exp glfw3dll.lib imgui*.obj
set includes=/I%glew_include_dir% /I%shader_dir% /I%glfw_include_dir% /I%glm_include_dir% /I%imgui_include_dir%

set opts=-DDEBUG -D_CRT_SECURE_NO_WARNINGS -FC -GR- -EHa- -nologo -Od -Zo -Oi -fp:fast -fp:except- -Gm- -GR- -EHa- -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 %includes%

pushd build

REM pushd dll
REM cl /LDd %debug_opts% %code%\app.cpp /link /LIBPATH:%lib_dir% %libs%
REM echo Copying dll build files to platform directories...
REM xcopy /s/y/v/q %cd%\app.* %glfw_dir%
REM xcopy /s/y/v/q %cd%\app.* %win32_dir%
REM popd

REM pushd win32
REM cl %debug_opts% %code%\win32_main.cpp -Feapp_win32 /link %libs%
REM popd

pushd glfw
REM TODO(lmk): The GLFW files are being included from outisde the project directory
cl %opts% %code%\glfw_win32_main.cpp -Feapp_glfw /link /LIBPATH:%lib_dir% %libs% 
popd
popd



