@echo off

set code=%cd%
set lib_dir=%code%\build\lib\release
set external=%code%\external
set freetype=%external%\freetype-2.13.0
set assimp=%external%\assimp-5.2.5
set sqlite=%external%\sqlite

set libs=User32.lib Opengl32.lib Gdi32.lib  glew32.lib glfw3dll.exp glfw3dll.lib imgui*.obj freetype.lib assimp-vc143-mt.lib Comdlg32.lib
set includes=/I%external% /I%freetype% /I%assimp% /I%sqlite%

set opts=-DDEBUG -DUSE_GLM -D_CRT_SECURE_NO_WARNINGS -FC -GR- -EHa- -nologo -Od -Zo -Oi -fp:fast -fp:except- -Gm- -GR- -EHa- -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 %includes%

REM pushd dll
REM cl /LDd %debug_opts% %code%\app.cpp /link /LIBPATH:%lib_dir% %libs%
REM echo Copying dll build files to platform directories...
REM xcopy /s/y/v/q %cd%\app.* %glfw_dir%
REM xcopy /s/y/v/q %cd%\app.* %win32_dir%
REM popd

pushd build
pushd glfw
REM To update
REM cl /c %opts% %code%\external\imgui-1.89.5\imgui*.cpp
cl %opts% %code%\glfw_win32_main.cpp -Feapp_glfw /link /LIBPATH:%lib_dir% %libs%

popd
popd



