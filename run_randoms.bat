SETLOCAL EnableDelayedExpansion

set "INSTANCES=esc128 tai256c wil100 tho150 lipa90b chr25a bur26a rou12"
set "TIMES=48370855 1305189380 106712720 677077515 46910305 338965 432875 24030"
set "REPS=10"
set "OUT_FOLDER=.\out\"

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

for /L %%A in (1,1,%i%) do (
    bio_solver.exe !INSTANCES%%A! random_walk %REPS% !TIMES%%A! > %OUT_FOLDER%rw_!INSTANCES%%A!.out
    bio_solver.exe !INSTANCES%%A! random_search %REPS% !TIMES%%A! > %OUT_FOLDER%rs_!INSTANCES%%A!.out
)