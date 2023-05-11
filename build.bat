@echo off

set code=%cd%
set shader_dir=%code%\shaders
set build_dir=%code%\build
set lib_dir=%build_dir%\lib
set win32_dir=%build_dir%\win32
set glfw_dir=%build_dir%\glfw
set glew_include_dir=%code%\glew

set libs=User32.lib Opengl32.lib Gdi32.lib glew32.lib

set release_opts=-FC -GR- -EHa- -nologo -O2 -Zo -Oi -fp:fast -fp:except- -Gm- -GR- -EHa- -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 /I%glew_include_dir% /I%shader_dir%

set debug_opts=-DDEBUG -FC -GR- -EHa- -nologo -Zi -fp:fast -fp:except- -Gm- -GR- -EHa- -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC /I%glew_include_dir% /I%shader_dir%

pushd build
pushd dll
cl /LDd %debug_opts% %code%\app.cpp /link /LIBPATH:%lib_dir% %libs%
echo Copying dll build files to platform directories...
xcopy /s/y/v/q %cd%\app.* %glfw_dir%
xcopy /s/y/v/q %cd%\app.* %win32_dir%
popd

pushd win32
cl %debug_opts% %code%\win32_main.cpp -Feapp_win32 /link %libs%
popd

pushd glfw
REM TODO(lmk): The GLFW files are being included from outisde the project directory
cl %debug_opts% %code%\glfw_win32_main.cpp -Feapp_glfw /link /LIBPATH:%lib_dir% %libs% glfw3dll.lib glfw3dll.exp
popd
popd



