@echo off
set "task=triangle"

g++ -Wl,--stack,1073741824 -std=gnu++20 -Wall -O2 -pipe -static -g -o "%task%.exe" grader.cpp "%task%.cpp"
if errorlevel 1 (
    echo.
    echo Compilation failed. Press any key to continue...
    pause >nul
)
