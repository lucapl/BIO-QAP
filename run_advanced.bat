set "INSTANCES=lipa20a lipa50a lipa90a esc16a esc32g esc128 tai50b tai100b tai150b tai64c tai256c"
set "REPS=300"
set "OUT_FOLDER=.\out\"
set "NO_IMPROV_ITERS=50"
set "TABU_TENURE=10"
set "TOP_PERCENT=0.25"
set "MAX_QUALITY_DROP=0.05"
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s tabu_search %REPS% %NO_IMPROV_ITERS% %TABU_TENURE% %TOP_PERCENT% %MAX_QUALITY_DROP% > %OUT_FOLDER%tabu_search_%%s.out
)

set "NO_IMPROV_CYCLES=10"
set "INITIAL_ACCEPTANCE=0.95"
set "TEMPERATURE_DECREASE=0.85"
set "CHAIN_LENGTH_PERCENT=0.25"
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s simulated_annealing %REPS% %INITIAL_ACCEPTANCE% %NO_IMPROV_CYCLES% %TEMPERATURE_DECREASE% %CHAIN_LENGTH_PERCENT% > %OUT_FOLDER%tabu_search_%%s.out
)
