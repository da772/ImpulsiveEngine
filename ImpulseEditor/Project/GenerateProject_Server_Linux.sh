#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
python ReloadProject.py

cd ..
./vendor/premake/premake5_linux gmake2 --os=linux --server
