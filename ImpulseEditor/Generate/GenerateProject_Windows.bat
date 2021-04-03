cd ..
python ReloadProject.py
cd ..
call vendor\premake\premake5.exe vs2019 --build-openal --build-engine --build-editor
PAUSE