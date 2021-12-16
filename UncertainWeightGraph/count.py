import os
import sys

testcases = os.listdir('testcases/')
testcases.sort()

cnt = 0

for testcase in testcases : 
    b = open('bruteforce/output/' + testcase, 'r')  
    t = open(sys.argv[1] + '/output/' + testcase, 'r')    
    if b.readline() == t.readline() : 
        cnt += 1

print(sys.argv[1] + ' : ' + str(cnt) + ' / ' + str(len(testcases)))