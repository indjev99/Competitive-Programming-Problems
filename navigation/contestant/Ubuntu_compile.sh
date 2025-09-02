#!/bin/bash

task="navigation"

if ! g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o "$task" grader.cpp "${task}.cpp"; then
    echo
    read -n 1 -s -r -p "Compilation failed. Press any key to continue..."
    echo
fi

