
@echo off
g++ -Ilibs/cfig main.cpp src/*.cpp libs/cfig/cfig.cpp -o main.exe
if not errorlevel 1 main.exe
