g++ -O2 -Wall tester.cpp fortuna.cpp -o fortuna.exe

for i in $(seq -w 1 20); do
  ./fortuna.exe < "fortuna.${i}.in" > "fortuna.${i}.sol"
done
