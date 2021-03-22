#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
python ReloadProject.py
cd ..
./vendor/bin/premake/premake5 xcode4 --hot-reload
./vendor/bin/premake/premake5 gmake2 --hot-reload
