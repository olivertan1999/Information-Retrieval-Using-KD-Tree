map1: map1.o csvparser.o kdtree.o search.o
	gcc -o map1 map1.o csvparser.o kdtree.o search.o -lm

csvparser.o: csvparser.c csvparser.h kdtree.h
	gcc -c -Wall csvparser.c
    
kdtree.o: kdtree.c kdtree.h
	gcc -c -Wall kdtree.c
    
search.o: search.c search.h kdtree.h csvparser.h
	gcc -c -Wall search.c
    
map1.o: map1.c kdtree.h
	gcc -c -Wall map1.c

map2: map2.o csvparser.o kdtree.o search.o
	gcc -o map2 map2.o csvparser.o kdtree.o search.o -lm
    
map2.o: map2.c kdtree.h
	gcc -c -Wall map2.c

