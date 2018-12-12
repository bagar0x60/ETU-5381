import time
import sys
import subprocess
import math
import matplotlib.pyplot as plt

PROCESS_COUNT_MAX = 20
ROUNDS_COUNT = 10
# command = "./Lab5/lab5 10000000 0"
command = "./Lab4/lab4 10000000 0"
# command = "./Lab3/lab3 10000 10000 0"
# command = "./Lab2/lab2 100000000 0"
stats = []
for p in range(1, PROCESS_COUNT_MAX + 1):
    m = math.inf
    for i in range(ROUNDS_COUNT):
        result = subprocess.run([f"{command} {p}"], shell=True, stdout=subprocess.PIPE)
        m = min(m, float(result.stdout))
    stats.append(m)
    print(p, m)
    
plt.plot(range(1, PROCESS_COUNT_MAX + 1), stats)
plt.show()