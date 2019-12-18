########################################################
CC=gcc
CFLAGS= -g -Wall -pedantic
EJS = main1 main2 main3 main4 main5 main6 main7
########################################################
OBJECTS1 = main1.o afnd.o transforma.o
OBJECTS2 = main2.o afnd.o transforma.o
OBJECTS3 = main3.o afnd.o transforma.o
OBJECTS4 = main4.o afnd.o transforma.o minimiza.o
OBJECTS5 = main5.o afnd.o transforma.o minimiza.o
OBJECTS6 = main6.o afnd.o transforma.o minimiza.o
OBJECTS7 = main7.o afnd.o transforma.o minimiza.o
########################################################

all: $(EJS) clear

### TRANSFORMA TEST MAINS ###

main1: $(OBJECTS1)
	$(CC) $(CFLAGS) -o main1 $(OBJECTS1)

main1.o: main1.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c main1.c

main2: $(OBJECTS2)
	$(CC) $(CFLAGS) -o main2 $(OBJECTS2)

main2.o: main2.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c main2.c

main3: $(OBJECTS3)
	$(CC) $(CFLAGS) -o main3 $(OBJECTS3)

main3.o: main3.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c main3.c

### MINIMIZA TEST MAINS ###

main4: $(OBJECTS4)
	$(CC) $(CFLAGS) -o main4 $(OBJECTS4)

main4.o: main4.c transforma.h minimiza.h afnd.h
	$(CC) $(CFLAGS) -c main4.c

main5: $(OBJECTS5)
	$(CC) $(CFLAGS) -o main5 $(OBJECTS5)

main5.o: main5.c transforma.h minimiza.h afnd.h
	$(CC) $(CFLAGS) -c main5.c

main6: $(OBJECTS6)
	$(CC) $(CFLAGS) -o main6 $(OBJECTS6)

main6.o: main6.c transforma.h minimiza.h afnd.h
	$(CC) $(CFLAGS) -c main6.c

main7: $(OBJECTS7)
	$(CC) $(CFLAGS) -o main7 $(OBJECTS7)

main7.o: main7.c transforma.h minimiza.h afnd.h
	$(CC) $(CFLAGS) -c main7.c

### LIBS ###

afnd.o: afnd.c afnd.h
	$(CC) -c afnd.c

transforma.o: transforma.c transforma.h afnd.h
	$(CC) $(CFLAGS) -c transforma.c

minimiza.o: minimiza.c transforma.h afnd.h minimiza.h
	$(CC) $(CFLAGS) -c minimiza.c

#####

clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS) *.dot *.png

runv:
	valgrind --leak-check=full ./main4
