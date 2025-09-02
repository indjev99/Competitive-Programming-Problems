@echo off
set "task=navigation"

g++ -Wl,--stack,536870912 -std=gnu++17 -Wall -O2 -pipe -static -g -o "%task%.exe" grader.cpp "%task%.cpp"
if errorlevel 1 (
    echo.
    echo Compilation failed. Press any key to continue...
    pause >nul
)

