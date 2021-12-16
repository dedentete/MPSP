for i in `seq 1 $1`
do
    t=$(printf "%04d" $i)
    ./main.out < ../testcases/$t.txt > output/$t.txt &
done
wait