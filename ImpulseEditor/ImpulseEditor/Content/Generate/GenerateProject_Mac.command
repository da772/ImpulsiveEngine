#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
python ReloadProject.py
cd ..
./vendor/premake/premake5_mac xcode4 --build-engine --build-editor
