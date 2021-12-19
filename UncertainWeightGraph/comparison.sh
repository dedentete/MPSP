cd expect
time sh solve.sh 1000
cd ..
python count.py expect
cd reduction
time sh solve.sh 1000 1000
cd ..
python count.py reduction
cd sampling
time sh solve.sh 1000 10
cd ..
python count.py sampling
cd sampling
time sh solve.sh 1000 100
cd ..
python count.py sampling
cd sampling
time sh solve.sh 1000 1000
cd ..
python count.py sampling
cd sampling
time sh solve.sh 1000 10000
cd ..
python count.py sampling
cd sampling
time sh solve.sh 1000 100000
cd ..
python count.py sampling