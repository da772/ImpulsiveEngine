#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
python ReloadProject.py
cd ..
./vendor/premake/premake5_mac xcode4 --hot-reload --build-engine
./vendor/premake/premake5_mac gmake2 --hot-reload --build-engine
