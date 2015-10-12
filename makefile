CC=gcc
CFLAGS=-I.

all: evolution

obj/server.o: src/server.c
	gcc -c -o obj/server.o src/server.c

obj/client.o: src/client.c
	gcc -c -o obj/client.o src/client.c

obj/network.a: obj/server.o obj/client.o
	ar cr obj/network.a obj/server.o obj/client.o

obj/management.o: src/management.c
	gcc -c -o obj/management.o src/management.c

obj/list.o: src/list.c
	gcc -c -o obj/list.o src/list.c

obj/hash.o: src/hash.c
	gcc -c -o obj/hash.o src/hash.c
	
obj/util.a: obj/list.o obj/hash.o
	ar cr obj/util.a obj/list.o obj/hash.o

obj/environment.o: src/environment.o
	gcc -c -o obj/environment.o src/environment.c

obj/character.o: src/character.c
	gcc -c -o obj/character.o src/character.c
	
obj/stats.o: src/stats.c
	gcc -c -o obj/stats.o src/stats.c

obj/user.o: src/user.c
	gcc -c -o obj/user.o src/user.c
	
obj/action.o: src/action.c
	gcc -c -o obj/action.o src/action.c
	
obj/room.o: src/room.c
	gcc -c -o obj/room.o src/room.c

evolution: evolution.c obj/network.a obj/management.o obj/environment.o obj/util.a obj/character.o obj/user.o obj/action.o obj/room.o obj/stats.o
	gcc -o evolution evolution.c obj/network.a obj/management.o obj/environment.o obj/util.a obj/character.o obj/user.o obj/action.o obj/room.o obj/stats.o -lpthread

clean:
	rm obj/*.o obj/*.a evolution