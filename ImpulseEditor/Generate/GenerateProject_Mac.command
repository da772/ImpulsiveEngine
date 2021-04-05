#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
python ReloadProject.py
cd ..
./vendor/premake/premake5_mac xcode4 --os=macosx --build-engine --build-editor
