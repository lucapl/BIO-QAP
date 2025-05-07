import numpy as np
import pandas as pd
from dataclasses import dataclass


@dataclass
class ProblemInfo:
    name: str
    size: int
    optimal_value: int
    optimum: any


def load_file(solver, instance, data_folder="./out/"):
    with open(f"{data_folder}{solver}_{instance}.out") as data_file:
        file_lines = data_file.readlines()
    _, size, optimum = file_lines[0].split()
    optimum = int(optimum)
    size = int(size)
    convert_to_ints = lambda line: list(map(int, line.split()))
    optimal_solution = np.array(convert_to_ints(file_lines[1]))
    repetitions = int(file_lines[2])

    problem = ProblemInfo(instance, size, optimum, optimal_solution)

    solutions = []
    values = []
    stats = []

    step = 4
    for i in range(3, len(file_lines), step):
        lines = file_lines[i:i+step]
        starting = np.array(convert_to_ints(lines[0]))
        final = np.array(convert_to_ints(lines[1]))
        start_value, end_value = convert_to_ints(lines[2])
        evals, iters, time = convert_to_ints(lines[3])

        solutions.append([starting, final])
        values.append([start_value, end_value])
        stats.append([evals, iters, time])

    return problem, np.array(solutions), np.array(values), np.array(stats)


def pandify(instances,solvers,solver_map):
    columns = ["repetition", "solver", "instance", "optimal_solution", "optimal_value", "initial_solution", "initial_value", "final_solution", "final_value", "iterations", "evaluations", "time"]
    data = {col:[] for col in columns}
    for solver in solvers:
        for instance in instances:
            problem, sols, vals, stats = load_file(solver, instance)
            reps = vals.shape[0]
            data["repetition"].extend(list(range(reps)))
            data["solver"].extend([solver_map[solver] for _ in range(reps)])
            data["instance"].extend([instance for _ in range(reps)])
            data["optimal_solution"].extend([problem.optimum for _ in range(reps)])
            data["optimal_value"].extend([problem.optimal_value for _ in range(reps)])
            data["initial_solution"].extend(sols[:,0,:])
            data["initial_value"].extend(vals[:,0])
            data["final_solution"].extend(sols[:,1,:])
            data["final_value"].extend(vals[:,1])
            data["iterations"].extend(stats[:,1])
            data["evaluations"].extend(stats[:,0])
            data["time"].extend(stats[:,2])
    return pd.DataFrame.from_dict(data)
