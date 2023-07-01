@echo off

set opts=-FC -GR- -EHa- -nologo -O2 -Zo -Oi -fp:fast -fp:except- -Gm- -GR- -EHa- -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7 /std:c++latest /EHsc
set code=%cd%
pushd .
cl %opts% %code%\asset_hash_collision.cpp -Feasset_hash_collision
popd
