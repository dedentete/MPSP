g++ -o reduction.out reduction.cpp
g++ -o main.out main.cpp
time ./reduction.out < ../testcases-large/10000.txt | time ./main.out 10 100