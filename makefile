########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic
EJS = main main2 main3 main4 main5
########################################################
OBJECTS1 = main.o afnd.o transforma.o
OBJECTS2 = main2.o afnd.o transforma.o
OBJECTS3 = main3.o afnd.o transforma.o
OBJECTS4 = main4.o afnd.o transforma.o minimiza.o
OBJECTS5 = main5.o afnd.o transforma.o minimiza.o
########################################################

all: $(EJS) clear

main: $(OBJECTS1)
	$(CC) $(CFLAGS) -o main $(OBJECTS1)

main.o: main.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c main.c

main2: $(OBJECTS2)
	$(CC) $(CFLAGS) -o main2 $(OBJECTS2)

main2.o: main.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c main2.c

main3: $(OBJECTS3)
	$(CC) $(CFLAGS) -o main3 $(OBJECTS3)

main3.o: main3.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c main3.c

main4: $(OBJECTS4)
	$(CC) $(CFLAGS) -o main4 $(OBJECTS4)

main4.o: main4.c transforma.h minimiza.h afnd.h
	$(CC) $(CFLAGS) -c main4.c

main5: $(OBJECTS5)
	$(CC) $(CFLAGS) -o main5 $(OBJECTS5)

main5.o: main5.c transforma.h minimiza.h afnd.h
	$(CC) $(CFLAGS) -c main5.c

afnd.o: afnd.c afnd.h
	$(CC) -c afnd.c

transforma.o: transforma.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c transforma.c

minimiza.o: minimiza.c transforma.h afnd.h minimiza.h
	$(CC) $(CFLAGS) -c minimiza.c

clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS) main *.dot *.png

runv:
	valgrind --leak-check=full ./main4
