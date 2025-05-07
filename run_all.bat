SETLOCAL EnableDelayedExpansion
set "INSTANCES=%*"
set "REPS=300"
set "OUT_FOLDER=.\out\"
mkdir %OUT_FOLDER%

rem run heuristics
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s heuristic %REPS% > %OUT_FOLDER%heuristic_%%s.out
)

set "LS_VERSION=steepest"
rem Local searches:
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s local_search %REPS% %LS_VERSION% > %OUT_FOLDER%ls_%LS_VERSION%_%%s.out
)
set "LS_VERSION=greedy"
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s local_search %REPS% %LS_VERSION% > %OUT_FOLDER%ls_%LS_VERSION%_%%s.out
)

rem get local search times:
py get_instance_times.py %INSTANCES% > _temp.txt || exit /b
set /p TIMES=<_temp.txt
del _temp.txt

rem Convert the lists into arrays
set i=0
for %%s in (%INSTANCES%) do (
   set /A i+=1
   set "INSTANCES!i!=%%s"
)
set i=0
for %%t in (%TIMES%) do (
   set /A i+=1
   set "TIMES!i!=%%t"
)

rem run random search and random walk
for /L %%A in (1,1,%i%) do (
    bio_solver.exe !INSTANCES%%A! random_walk %REPS% !TIMES%%A! > %OUT_FOLDER%rw_!INSTANCES%%A!.out
    bio_solver.exe !INSTANCES%%A! random_search %REPS% !TIMES%%A! > %OUT_FOLDER%rs_!INSTANCES%%A!.out
)
