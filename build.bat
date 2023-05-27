@echo off

set code=%cd%
set shader_dir=%code%\shaders
set build_dir=%code%\build
set lib_dir=%build_dir%\lib\release
set win32_dir=%build_dir%\win32
set glfw_dir=%build_dir%\glfw
set glew_include_dir=%code%\glew

set libs=User32.lib Opengl32.lib Gdi32.lib

set opts=-DDEBUG -D_CRT_SECURE_NO_WARNINGS -FC -GR- -EHa- -nologo -Od -Zo -Oi -fp:fast -fp:except- -Gm- -GR- -EHa- -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 /I%glew_include_dir% /I%shader_dir%

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
cl %opts% %code%\glfw_win32_main.cpp -Feapp_glfw /link /LIBPATH:%lib_dir% %libs% glew32.lib glfw3dll.exp glfw3dll.lib
popd
popd



