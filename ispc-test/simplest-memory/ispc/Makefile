CXX = clang -m64
CXXFLAGS = -Iobjs/ -O3 -Wall -lrt
ISPC = ispc
ISPCFLAGS = -O3 --arch=x86-64 --target=avx

default: main

.PHONY: dirs clean

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ main

main: dirs  objs/main.o objs/main_ispc.o
	$(CXX) $(CXXFLAGS) -o $@ objs/main.o objs/main_ispc.o

objs/main.o: main.c objs/main_ispc.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

objs/%_ispc.h objs/%_ispc.o: %.ispc
	$(ISPC) $(ISPCFLAGS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h
