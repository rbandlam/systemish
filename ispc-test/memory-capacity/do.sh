# Remove array A's shm-id
sudo ipcrm -M 3185

# Create C++ file
ispc main.ispc --emit-c++ --target=generic-16 -o main-ispc.cpp --c++-include-file=generic-16.h

gcc -I ~/ISPC/ispc/examples/intrinsics/ main-ispc.cpp main.c -o main -lrt -lm


