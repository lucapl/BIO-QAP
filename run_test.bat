set "MAIN=test.cpp"
set "CORE_LIB=src/qap.cpp src/util.cpp"
set "PROGRAM_NAME=delta_test.exe"
g++ -Iinclude %MAIN% %CORE_LIB% -o %PROGRAM_NAME% && .\%PROGRAM_NAME% %*