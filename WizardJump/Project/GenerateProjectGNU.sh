#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
python ReloadProject.py

cd ..
./vendor/bin/premake/premake5_mac gmake2 --os=linux
