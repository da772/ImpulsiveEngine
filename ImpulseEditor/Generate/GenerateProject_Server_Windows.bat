cd ..
python ReloadProject.py
cd ..
call vendor\premake\premake5.exe vs2019 --server --build-engine
PAUSE