%~d0
cd %~dp0
cd ..
xcopy "BuildTarget.lua" "../premake5.lua*" /K /D /H /Y /I
cd ..
call vendor\premake\premake5.exe %*
PAUSE