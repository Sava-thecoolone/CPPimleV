@echo off
set "arg=%1"
if "%arg:~0,9%" == "renderers" (g++ "%1.cpp" -o "%1.dll" -shared -lraylib) else (g++ "%1.cpp" -o "%1.dll" -shared)