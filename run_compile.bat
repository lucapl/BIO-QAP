set "MAIN=main.cpp"
set "CORE_LIB=src/qap.cpp src/solvers.cpp src/util.cpp"
set "EXPERIMENTS=src/experiments/experiment.cpp"
set "PROGRAM_NAME=bio_solver.exe"
g++ -O3 -Iinclude %MAIN% %CORE_LIB% %EXPERIMENTS% -o %PROGRAM_NAME% && .\%PROGRAM_NAME% %*