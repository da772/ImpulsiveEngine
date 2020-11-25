#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
python ReloadProject.py
cd ..
./vendor/bin/premake/premake5 gmake2 --os=linux
