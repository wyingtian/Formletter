OBJS = process.o fl.o wordstore13.o get_record.o mailmerge.o 
CC = gcc
DEBUG = -g

fl : $(OBJS)
	$(CC)  $(OBJS) -o fl

fl.o : fl.h fl.c ws13.h get_record.c mailmerge.c
	$(CC) $(DEBUG) -c fl.c

process.o : process.c fl.h 
	$(CC) $(DEBUG) -c process.c

wordstore13.o : wordstore13.c ws13.h
	$(CC) $(DEBUG) -c wordstore13.c

get_record.o : get_record.c fl.h 
	$(CC) $(DEBUG) -c get_record.c

mailmerge.o : mailmerge.c fl.h 
	$(CC) $(DEBUG) -c mailmerge.c		

clean:
	\rm *.o *~ p1

