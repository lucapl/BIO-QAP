set "INSTANCES=lipa20a lipa50a lipa90a esc16a esc32g esc128 tai50b tai100b tai150b tai64c tai256c"
set "REPS=300"
set "OUT_FOLDER=.\out\"
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s heuristic %REPS% > %OUT_FOLDER%heuristic_%%s.out
)
