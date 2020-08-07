#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cp -rf ./Podfile_ios ../Podfile
python ReloadProject.py
cd ..
./vendor/bin/premake/premake5 xcode4 --os=ios
pod install