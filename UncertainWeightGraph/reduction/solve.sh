g++ -o reduction.out reduction.cpp
g++ -o main.out main.cpp
for i in `seq 1 $1`
do
    t=$(printf "%04d" $i)
    time ./reduction.out < ../testcases/$t.txt | ./main.out 20 1000 > output/$t.txt
done