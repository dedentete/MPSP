n=$(($1-1))
for i in `seq 0 $n`
do
    t=$(printf "%04d" $i)
    ./reduction.out < ../testcases/$t.txt | ./main.out 20 $2 > output/$t.txt &
done
wait