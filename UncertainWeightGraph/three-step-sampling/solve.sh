g++ -o main.out main.cpp
for i in `seq 1 $1`
do
    t=$(printf "%04d" $i)
    time ./main.out 20 1000 1000 < ../testcases/$t.txt > output/$t.txt
done