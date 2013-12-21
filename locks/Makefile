all:
	gcc -O2 -o spinlock spinlock.c -lpthread -lrt
	gcc -O2 -o bouncing bouncing.c -lpthread -lrt -D_GNU_SOURCE
	gcc -O2 -o manylock manylock.c -lpthread -lrt -D_GNU_SOURCE

