cd ..
python ReloadProject.py
cd ..
call vendor\premake\premake5.exe vs2019 --build-openal --target-name=MyProject --build-engine
PAUSE