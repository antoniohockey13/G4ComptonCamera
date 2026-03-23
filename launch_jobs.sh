#!/bin/bash

NJOBS=20
EVENTS=100000000
MACRODIR=~/repos/G4ComptonCamera

for i in $(seq 1 $NJOBS); do
    SEED1=$((1039348 + i*101))
    SEED2=$((6582264 + i*211))
    MACRO="${MACRODIR}/run_${i}.mac"

    cat > "$MACRO" <<EOF
/gps/hist/inter Lin
/run/initialize
/run/printProgress 10000
/random/setSeeds $SEED1 $SEED2
/run/beamOn $EVENTS
EOF

    ./ComptCamera "$MACRO" > "log_${i}.txt" 2>&1 &
done

wait
