g++ -o reduction.out reduction.cpp
g++ -o main.out main.cpp
./reduction.out < ../testcases/0000.txt | time ./main.out 20 10000