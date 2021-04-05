cd ..
python ReloadProject.py
cd ..
call vendor\premake\premake5.exe vs2019 --os=android --build-openal --build-engine --package
PAUSE