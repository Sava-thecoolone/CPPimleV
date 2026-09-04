@echo off
for %%i in (sorts/*.cpp) do (g++ "sorts/%%i" -o "sorts/%%~ni.dll" -shared)
for %%i in (shuffles/*.cpp) do (g++ "shuffles/%%i" -o "shuffles/%%~ni.dll" -shared)
for %%i in (renderers/*.cpp) do (g++ "renderers/%%i" -o "renderers/%%~ni.dll" -shared -lraylib)