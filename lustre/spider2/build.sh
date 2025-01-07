(cd $(dirname "$0") && 
heptc lib.epi &&
heptc -target c robot.ept
)
make -C build spider2 -j 3


