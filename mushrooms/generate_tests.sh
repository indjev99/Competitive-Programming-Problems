#!/bin/bash

rm -rf tests
mkdir -p bin
mkdir -p extras
mkdir -p tests


pushd testgen
g++ testgen.cpp -o testgen -O1 -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector
popd
g++ solutions/author_checks.cpp -o bin/author_checks -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector
./testgen/testgen
cat grade.properties

rm ../AB11_mushrooms.zip
zip -r ../AB11_mushrooms.zip solutions/ extras/ testgen/testgen.cpp testgen/testlib.h testgen/common.h tests/*.in tests/*.out grade.properties generate_tests.sh statement
git add solutions/ extras/ testgen/ tests/*.in tests/*.out grade.properties generate_tests.sh
