#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
cp -rf ./Podfile_ios ../Podfile
python ReloadProject.py
cd ..
./vendor/premake/premake5_mac xcode4 --os=ios
echo Loading Pod Files...
pod install
#delete pod static lib from sandbox!
