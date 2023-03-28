@echo off

set opts=-DDEBUG -FC -GR- -EHa- -nologo -O2 -Zo -Oi -fp:fast -fp:except- -Gm- -GR- -EHa- -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7
set libs=User32.lib Opengl32.lib Gdi32.lib

set code=%cd%
pushd build
cl %opts% %code%\win32_main.cpp -Fetest /link %libs%
popd
