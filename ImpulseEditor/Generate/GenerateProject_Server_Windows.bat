cd ..
python ReloadProject.py
cd ..
call vendor\premake\premake5.exe vs2019 --os=windows --server --build-engine
PAUSE