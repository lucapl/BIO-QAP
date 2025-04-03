set "INSTANCES=esc128 tai256c wil100 tho150 lipa90b chr25a bur26a rou12"
set "REPS=10"
set "OUT_FOLDER=.\out\"
set "LS_VERSION=steepest"
mkdir %OUT_FOLDER%
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s local_search %REPS% %LS_VERSION% > %OUT_FOLDER%ls_%LS_VERSION%_%%s.out
)

set "LS_VERSION=greedy"
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s local_search %REPS% %LS_VERSION% > %OUT_FOLDER%ls_%LS_VERSION%_%%s.out
)