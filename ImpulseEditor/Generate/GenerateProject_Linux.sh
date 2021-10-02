#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
cp -f "BuildTarget.lua" "../premake5.lua"
cd ..
./vendor/premake/premake5_linux gmake2 --os=linux --build-engine --build-editor --disable-nfd
