CC=gcc 
CFLAGS=-Wpedantic -std=gnu99 -Wall -g

all: drawDir
	
drawDir: drawDir.c FSreport.h
	$(CC) $(CFLAGS) drawDir.c -o drawDir

drawDir.o: drawDir.c FSreport.h
	$(CC) $(CFLAGS) -c drawDir.c -o drawDir.o

clean:
	rm -f *.o FSreport drawDir
