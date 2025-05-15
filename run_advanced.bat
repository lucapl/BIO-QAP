set "INSTANCES=lipa20a lipa50a lipa90a esc16a esc32g esc128 tai50b tai100b tai150b tai64c tai256c"
set "REPS=20"
set "OUT_FOLDER=.\out\"
set "NO_IMPROV_ITERS=250"
set "TABU_TENURE=0.25"
set "TOP_PERCENT=0.2"
set "MAX_QUALITY_DROP=0.1"
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s tabu_search %REPS% %NO_IMPROV_ITERS% %TABU_TENURE% %TOP_PERCENT% %MAX_QUALITY_DROP% > %OUT_FOLDER%tabu_search_%%s.out
)

set "NO_IMPROV_CYCLES=20"
set "INITIAL_ACCEPTANCE=0.95"
set "TEMPERATURE_DECREASE=0.9"
set "CHAIN_LENGTH_PERCENT=2.0"
for %%s in (%INSTANCES%) do (
    bio_solver.exe %%s simulated_annealing %REPS% %NO_IMPROV_CYCLES% %INITIAL_ACCEPTANCE% %TEMPERATURE_DECREASE% %CHAIN_LENGTH_PERCENT% > %OUT_FOLDER%simulated_annealing_%%s.out
)
