
clear
rm a.out -f
g++ a.cpp SFMT.c  -O3 -fopenmp
#gcc -O3 -finline-functions -fomit-frame-pointer -DNDEBUG -fno-strict-aliasing --param max-inline-insns-single=1800 -Wmissing-prototypes -Wall  -std=c99 --param inline-unit-growth=500 --param large-function-growth=900 -DSFMT_MEXP=19937 \
#a.c SFMT.c
export OMP_NUM_THREADS=5

time ./a.out
