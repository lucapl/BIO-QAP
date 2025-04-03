set "MAIN=test.cpp"
set "CORE_LIB=src/qap.cpp src/util.cpp"
set "EXPERIMENTS=src/experiments/experiment.cpp"
set "PROGRAM_NAME=delta_test.exe"
g++ -Iinclude %MAIN% %CORE_LIB% %EXPERIMENTS% -o %PROGRAM_NAME% && .\%PROGRAM_NAME% %*