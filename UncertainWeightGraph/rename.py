import os
import subprocess

testcases = os.listdir('testcases/')
testcases.sort()

remain = list()

for testcase in testcases : 
    number_of_lines = int(subprocess.check_output(['wc', '-l', 'bruteforce/output/' + testcase]).decode().split(' ')[0])
    if number_of_lines == 2 : 
        os.remove('testcases/' + testcase)
        os.remove('bruteforce/output/' + testcase)
        continue
    remain.append(testcase)

for i in range(len(remain)) :
    os.rename('testcases/' + remain[i], 'testcases/' + str(i).zfill(4) + '.txt')
    os.rename('bruteforce/output/' + remain[i], 'bruteforce/output/' + str(i).zfill(4) + '.txt')