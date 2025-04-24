import numpy as np
import sys
from util import load_file


if __name__ == '__main__':
    instances = sys.argv[1:]
    solvers = ["ls_greedy", "ls_steepest"]
    times = []
    for instance in instances:
        tims = []
        for solver in solvers:
            _, _, _, stats = load_file(solver, instance)
            tims.append(np.mean(stats, 0)[2])
        times.append(tims)
    times_mean = np.mean(times, 1).astype(np.int64)
    print(*times_mean)
