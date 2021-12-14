import os
import subprocess

testcases = os.listdir('testcases/')
testcases.sort()

cnt = 0

for testcase in testcases : 
    if testcase == 'test.txt' :
        continue
    number_of_lines = int(subprocess.check_output(['wc', '-l', 'bruteforce/output/' + testcase]).decode().split(' ')[0])
    if number_of_lines == 2 : 
        continue
    print(testcase) 
    print(number_of_lines)
    cnt += 1
print(cnt)