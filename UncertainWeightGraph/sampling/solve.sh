n=$(($1-1))
for i in `seq 0 $n`
do
    t=$(printf "%04d" $i)
    ./main.out $2 < ../testcases/$t.txt > output/$t.txt &
done
wait