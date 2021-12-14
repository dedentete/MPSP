g++ -o generator.out generator.cpp
for i in `seq 1 $1`
do
    t=$(printf "%04d" $i)
    ./generator.out 10 20 2 $i > testcases/$t.txt
done