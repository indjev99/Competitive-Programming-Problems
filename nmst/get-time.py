import resource
import subprocess
import random
import sys

[arg0, solution, *tests] = sys.argv

worst = 0.0
total = 0.0
n = 0
for i in range(10):
    random.shuffle(tests)
    for t in tests:
        info1 = resource.getrusage(resource.RUSAGE_CHILDREN)
        with open(t, 'rb', buffering=0) as f:
            subprocess.check_call([solution], stdin=f.fileno(), stdout=subprocess.DEVNULL)
        info2 = resource.getrusage(resource.RUSAGE_CHILDREN)
        cur = (info2.ru_utime - info1.ru_utime) + (info2.ru_stime - info1.ru_stime)
        if cur > worst:
            worst = cur
            print(worst)
        total += cur
        n += 1

print('avg', total / n)
