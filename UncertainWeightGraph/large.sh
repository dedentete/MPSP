cd three-step-sampling-large
g++ -o main.out main.cpp
time timeout 3600 ./main.out 20 10 10 < ../testcases-large/100.txt > output/100.txt
time timeout 3600 ./main.out 20 100 100 < ../testcases-large/100.txt > output/100.txt
time timeout 3600 ./main.out 20 1000 1000 < ../testcases-large/100.txt > output/100.txt
time timeout 3600 ./main.out 20 10 10 < ../testcases-large/1000.txt > output/1000.txt
time timeout 3600 ./main.out 20 100 100 < ../testcases-large/1000.txt > output/1000.txt
time timeout 3600 ./main.out 20 1000 1000 < ../testcases-large/1000.txt > output/1000.txt
time timeout 3600 ./main.out 20 10 10 < ../testcases-large/10000.txt > output/10000.txt
time timeout 3600 ./main.out 20 100 100 < ../testcases-large/10000.txt > output/10000.txt
time timeout 3600 ./main.out 20 1000 1000 < ../testcases-large/10000.txt > output/10000.txt
