%~d0
cd %~dp0
cd ..
python ReloadProject.py
cd ..
call vendor\bin\premake\premake5.exe vs2019 --hot-reload --build-openal
PAUSE