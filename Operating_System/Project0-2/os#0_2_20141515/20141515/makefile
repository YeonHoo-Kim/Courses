TARGET = testlib
OBJS = main.o list.o bitmap.o hash.o
CC = gcc

$(TARGET) : $(OBJS)
					$(CC) -lm -o $(TARGET) $(OBJS)

main.o : main.c list.h hash.h bitmap.h
				gcc -c main.c
list.o : list.c list.h
				gcc -c list.c
hash.o : hash.c hash.h
				gcc -c hash.c
bitmap.o : bitmap.c bitmap.h
				gcc -c bitmap.c
clean:
	rm $(OBJS) $(TARGET)
