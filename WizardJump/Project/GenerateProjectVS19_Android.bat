cd ..
python ReloadProject.py
cd ..
call vendor\bin\premake\premake5.exe vs2019 --os=android --build-openal
PAUSE